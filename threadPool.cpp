#include "threadPool.h"

ThreadPool::ThreadPool(int nums) :isStop(false) {
	for (int i = 0; i < nums; i++) {
		pool.emplace_back([this]() {
			this->task();
			});
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
		// ��������
		std::function<void()> task;
		
		// �ӻ��������ȡ��һ������
		{
			std::unique_lock<std::mutex> lock(mtx);

			cv.wait(lock, [this] {return this->isStop || !this->buffer.empty(); });

			if (isStop && buffer.empty()) return;
			task = std::move(this->buffer.front());
			this->buffer.pop();
		}

		// ִ������
		task();
	}
}