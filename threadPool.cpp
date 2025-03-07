#include "threadPool.h"

ThreadPool::ThreadPool(int nums) :isStop(false), min_threads(nums), max_threads(2*nums) {
	for (int i = 0; i < nums; i++) {
		pool.emplace_back([this]() {task(); });
	}
}

ThreadPool::~ThreadPool() {
	// �ı�isStop������ʾ�̳߳��Ѿ�ֹͣ
	{
		// ����unique_lock���л������
		std::unique_lock<std::mutex> lock(mtx);
		isStop = true;
	}

	// ֪ͨ�����������߳�
	cv.notify_all();
	
	// ��֤���е��߳�ִ����
	for (std::thread& each_thread : pool) {
		each_thread.join();
	}
}

void ThreadPool::task() {
	while (true) {
		// �������Ϊ���Ҳ�Ϊ��С�߳�����̬�رյ�ǰ�߳�
		if (buffer.empty() && total_threads > min_threads) {
			total_threads--;
			return;
		}

		// ��������
		std::function<void()> task;
		
		// �ӻ��������ȡ��һ������
		{
			std::unique_lock<std::mutex> lock(mtx);
			// �������Ϊ�����������
			cv.wait(lock, [this] {return this->isStop || !this->buffer.empty(); });
			
			// �̳߳�ֹͣ������ȫ��ִ������ر��߳�
			if (isStop && buffer.empty()) return;

			task = this->buffer.top().task;
			// ���ȼ���ʾ
			std::cout << "Executing task with priority: "
				<< static_cast<int>(this->buffer.top().priority) << std::endl;
			this->buffer.pop();
			// ��Ծ�߳�����һ
			active_threads++;
		}

		// ִ������
		task();

		// ��Ծ�߳�����һ
		active_threads--;
	}
}

void ThreadPool::controlThread() {
	// ÿ1����һ��
	std::this_thread::sleep_for(std::chrono::seconds(1));
	// ������������������ڵ�ǰ���߳����ҵ�ǰ���߳���û�дﵽ��ֵ�������߳�
	if (total_threads < max_threads && buffer.size() > total_threads) {
		total_threads++;
		pool.emplace_back([this]() {return task(); });
	}
}