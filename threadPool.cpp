#include "threadPool.h"

ThreadPool::ThreadPool(int nums) :isStop(false) {
	for (int i = 0; i < nums; i++) {
		pool.emplace_back([this]() {
			this->task();
			});
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
		// 定义任务
		std::function<void()> task;
		
		// 从缓冲队列中取得一个任务
		{
			std::unique_lock<std::mutex> lock(mtx);

			cv.wait(lock, [this] {return this->isStop || !this->buffer.empty(); });

			if (isStop && buffer.empty()) return;
			task = std::move(this->buffer.front());
			this->buffer.pop();
		}

		// 执行任务
		task();
	}
}