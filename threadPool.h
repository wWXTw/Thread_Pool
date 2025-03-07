#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <future>
#include <functional>
#include <atomic>
#include <chrono>
#include <condition_variable>

// �����������ȼ�
enum class TaskPriority
{
	Low = 1,
	Medium = 2,
	High = 3
};

// ����������
template <typename T>
class Task_t {
public:
	// ��������
	T task;
	// �������ȼ�
	TaskPriority priority;

	// �ȽϷ�ʽ
	bool operator<(const Task_t<T>& other) const {
		return this->priority < other.priority;
	}
};


// �����̳߳���
class ThreadPool {
private:
	// �̵߳�ִ������
	void task();
	// ��̬�����߳���Ŀ�ĺ���
	void controlThread();
	// ��С�߳���
	const int min_threads;
	// ����߳���
	const int max_threads;
	// ��ǰ��Ծ�߳���
	std::atomic<int> active_threads;
	// ��ǰ�߳���
	std::atomic<int> total_threads;
	// ��ʾ��ǰ�̳߳��ǲ���ֹͣ
	bool isStop;
	// ��������
	std::condition_variable cv;
	// ������
	std::mutex mtx;
	// �̳߳�(���ڴ�Ŷ���߳�)
	std::vector<std::thread> pool;
	// �������ȶ���
	std::priority_queue<Task_t<std::function<void()>>> buffer;
public:
	// ���캯��
	ThreadPool(int nums);
	// ������񵽻������(ģ�溯��) 
	template <typename F, typename ...Arg>
	auto addTask(F&& f, TaskPriority priority, Arg&& ...arg) -> std::future<typename std::result_of<F(Arg...)>::type>;
	// �������ȼ���addTask���غ���
	template <typename F, typename ...Arg>
	auto addTask(F&& f, Arg&& ...arg) -> std::future<typename std::result_of<F(Arg...)>::type>;
	// ��������
	~ThreadPool();
};

template <typename F, typename ...Arg>
auto ThreadPool::addTask(F&& f, TaskPriority priority, Arg&& ...arg) -> std::future<typename std::result_of<F(Arg...)>::type> {
	// ���fִ�к����������
	using functype = typename std::result_of<F(Arg...)>::type;

	// ���һ������ָ�� ָ��һ������װΪfunctype()��task
	auto task = std::make_shared<std::packaged_task<functype()>>(
		std::bind(std::forward<F>(f), std::forward<Arg>(arg)...)
	);

	// ��ȡfuture
	std::future<functype> res = task->get_future();

	// ��������ӵ�����
	{
		std::unique_lock<std::mutex> lock(mtx);
		if (isStop) {
			throw std::runtime_error("�̳߳��Ѿ�ֹͣ");
		}

		// �������
		Task_t<std::function<void()>> t;
		t.priority = priority;
		t.task = [task]() { (*task)(); };
		buffer.push(t);
	}

	// ֪ͨ�߳�ȥִ������
	cv.notify_one();

	return res;
}

template<typename F, typename ...Arg>
auto ThreadPool::addTask(F&& f, Arg && ...arg) -> std::future<typename std::result_of<F(Arg ...)>::type>
{
	// ���������Ĭ�����ȶ�ΪMedium
	return addTask(std::forward<F>(f), TaskPriority::Medium, std::forward<Arg>(arg)...);
}