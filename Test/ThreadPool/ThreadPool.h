#pragma once

#include <queue>
#include <functional>
#include <type_traits>
#include <atomic>
#include <vector>
#include <thread>
#include <future>

class MThreadPool
{
public:
	MThreadPool(unsigned count_of_threads = 0);
	~MThreadPool();

	template <typename Function, typename ...Args>
	std::future<typename std::result_of<Function(Args...)>::type>
	Execute(Function function, Args && ... arguments);
private:
	std::vector<std::thread> m_threads;
	std::queue<std::function<void()>> m_available_tasks;
	std::mutex m_queue_mutex, m_log_mutex;
	std::condition_variable m_notifier, m_finish_indicator;

	bool m_stop_flag{ false };
	std::atomic<unsigned> m_active_threads_count { 0 };

	void RunExecution();

	void Log(const std::string &message);
};

template<typename Function, typename ...Args>
std::future<typename std::result_of<Function(Args...)>::type>
MThreadPool::Execute(Function function, Args&& ...arguments)
{
	using return_type = typename std::result_of<Function(Args...)>::type;

	auto wrapped_task = std::make_shared< std::packaged_task<return_type()>>(
		[function, &arguments...] { return function(std::forward<Args>(arguments)...); }
		);

	std::future<return_type> result = wrapped_task->get_future();
	{
		std::lock_guard<std::mutex> lock(m_queue_mutex);

		m_available_tasks.emplace([wrapped_task]() { (*wrapped_task)(); });
	}
	m_notifier.notify_one();
	return result;
}
