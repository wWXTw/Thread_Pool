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

// 定义任务优先级
enum class TaskPriority
{
	Low = 1,
	Medium = 2,
	High = 3
};

// 定义任务类
template <typename T>
class Task_t {
public:
	// 函数任务
	T task;
	// 任务优先级
	TaskPriority priority;

	// 比较方式
	bool operator<(const Task_t<T>& other) const {
		return this->priority < other.priority;
	}
};


// 定义线程池类
class ThreadPool {
private:
	// 线程的执行内容
	void task();
	// 动态控制线程数目的函数
	void controlThread();
	// 最小线程数
	const int min_threads;
	// 最大线程数
	const int max_threads;
	// 当前活跃线程数
	std::atomic<int> active_threads;
	// 当前线程数
	std::atomic<int> total_threads;
	// 表示当前线程池是不是停止
	bool isStop;
	// 条件变量
	std::condition_variable cv;
	// 互斥锁
	std::mutex mtx;
	// 线程池(用于存放多个线程)
	std::vector<std::thread> pool;
	// 缓冲优先队列
	std::priority_queue<Task_t<std::function<void()>>> buffer;
public:
	// 构造函数
	ThreadPool(int nums);
	// 添加任务到缓冲队列(模版函数) 
	template <typename F, typename ...Arg>
	auto addTask(F&& f, TaskPriority priority, Arg&& ...arg) -> std::future<typename std::result_of<F(Arg...)>::type>;
	// 不带优先级的addTask重载函数
	template <typename F, typename ...Arg>
	auto addTask(F&& f, Arg&& ...arg) -> std::future<typename std::result_of<F(Arg...)>::type>;
	// 析构函数
	~ThreadPool();
};

template <typename F, typename ...Arg>
auto ThreadPool::addTask(F&& f, TaskPriority priority, Arg&& ...arg) -> std::future<typename std::result_of<F(Arg...)>::type> {
	// 获得f执行后的数据类型
	using functype = typename std::result_of<F(Arg...)>::type;

	// 获得一个智能指针 指向一个被包装为functype()的task
	auto task = std::make_shared<std::packaged_task<functype()>>(
		std::bind(std::forward<F>(f), std::forward<Arg>(arg)...)
	);

	// 获取future
	std::future<functype> res = task->get_future();

	// 将任务添加到队列
	{
		std::unique_lock<std::mutex> lock(mtx);
		if (isStop) {
			throw std::runtime_error("线程池已经停止");
		}

		// 正常添加
		Task_t<std::function<void()>> t;
		t.priority = priority;
		t.task = [task]() { (*task)(); };
		buffer.push(t);
	}

	// 通知线程去执行任务
	cv.notify_one();

	return res;
}

template<typename F, typename ...Arg>
auto ThreadPool::addTask(F&& f, Arg && ...arg) -> std::future<typename std::result_of<F(Arg ...)>::type>
{
	// 不传入参数默认优先度为Medium
	return addTask(std::forward<F>(f), TaskPriority::Medium, std::forward<Arg>(arg)...);
}