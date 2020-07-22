#include <future>
#include <vector>
#include <thread>
#include <queue>
#include <condition_variable>
#include <functional>
#pragma once

class ThreadPool {
public:
	explicit ThreadPool(size_t poolSize);
	~ThreadPool();
	template <class Func, class... Args>
	auto exec(Func func, Args... args)->std::future<decltype(func(args...))> {
		auto task = std::make_shared<std::packaged_task<decltype(func(args...))()>>
				(std::bind(func, args...));
		auto future = task->get_future();
		std::lock_guard<std::mutex> lock(m_);
		tasks_.push([task]() {
			(*task)();
		});
		tasksEmpty_.notify_one();
		return future;
	}
private:
	size_t poolSize_;
	std::queue<std::function<void()>> tasks_;
	std::vector<std::thread> pool_;
	std::condition_variable tasksEmpty_;
	std::mutex m_;
	std::atomic<bool> work_;
	void handler();
};

ThreadPool::ThreadPool(size_t poolSize):
poolSize_(poolSize),
work_(true) {
	for (size_t i = 0; i < poolSize_; ++i) {
		std::thread newThread(&ThreadPool::handler, this);
		newThread.detach();
		pool_.push_back(std::move(newThread));
	}
}

ThreadPool::~ThreadPool() {
	work_ = false;
	tasksEmpty_.notify_all();
}

void ThreadPool::handler() {
	while (true) {
		std::unique_lock<std::mutex> lock(m_);
		tasksEmpty_.wait(lock, [this] {
			return !(tasks_.empty() && work_);
		});
		if (!work_) {
			break;
		}
 		auto task = tasks_.front();
		tasks_.pop();
		task();
	}
}
