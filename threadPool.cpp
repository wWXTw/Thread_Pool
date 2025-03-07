#include "threadPool.h"

ThreadPool::ThreadPool(int nums) :isStop(false), min_threads(nums), max_threads(2*nums) {
	for (int i = 0; i < nums; i++) {
		pool.emplace_back([this]() {task(); });
	}
}

ThreadPool::~ThreadPool() {
	// 改变isStop变量表示线程池已经停止
	{
		// 利用unique_lock进行互斥控制
		std::unique_lock<std::mutex> lock(mtx);
		isStop = true;
	}

	// 通知所有阻塞的线程
	cv.notify_all();
	
	// 保证所有的线程执行完
	for (std::thread& each_thread : pool) {
		each_thread.join();
	}
}

void ThreadPool::task() {
	while (true) {
		// 任务队列为空且不为最小线程数则动态关闭当前线程
		if (buffer.empty() && total_threads > min_threads) {
			total_threads--;
			return;
		}

		// 定义任务
		std::function<void()> task;
		
		// 从缓冲队列中取得一个任务
		{
			std::unique_lock<std::mutex> lock(mtx);
			// 缓冲队列为空则进行阻塞
			cv.wait(lock, [this] {return this->isStop || !this->buffer.empty(); });
			
			// 线程池停止且任务全部执行完则关闭线程
			if (isStop && buffer.empty()) return;

			task = this->buffer.top().task;
			// 优先级显示
			std::cout << "Executing task with priority: "
				<< static_cast<int>(this->buffer.top().priority) << std::endl;
			this->buffer.pop();
			// 活跃线程数加一
			active_threads++;
		}

		// 执行任务
		task();

		// 活跃线程数减一
		active_threads--;
	}
}

void ThreadPool::controlThread() {
	// 每1秒检查一次
	std::this_thread::sleep_for(std::chrono::seconds(1));
	// 如果队列中任务数大于当前总线程数且当前总线程数没有达到阈值则增加线程
	if (total_threads < max_threads && buffer.size() > total_threads) {
		total_threads++;
		pool.emplace_back([this]() {return task(); });
	}
}