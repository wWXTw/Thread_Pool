#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <future>
#include <functional>
#include <condition_variable>

class ThreadPool {
private:
	// 线程的执行内容
	void task();
	// 表示当前线程池是不是停止
	bool isStop;
	// 条件变量
	std::condition_variable cv;
	// 互斥锁
	std::mutex mtx;
	// 线程池(用于存放多个线程)
	std::vector<std::thread> pool;
	// 缓冲队列
	std::queue<std::function<void()>> buffer;
public:
	// 构造函数
	ThreadPool(int nums);
	// 添加任务到缓冲队列(模版函数) 
	template <typename F, typename ...Arg>
	auto addTask(F&& f, Arg&& ...arg) -> std::future<typename std::result_of<F(Arg...)>::type>;
	// 析构函数
	~ThreadPool();
};

template <typename F, typename ...Arg>
auto ThreadPool::addTask(F&& f, Arg&& ...arg) -> std::future<typename std::result_of<F(Arg...)>::type> {
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
		buffer.emplace([task]() {
			(*task)();
			});
	}

	// 通知线程去执行任务
	cv.notify_one();

	return res;
}