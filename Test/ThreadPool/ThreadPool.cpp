#include "ThreadPool.h"

#include <iostream>
#include <string>

MThreadPool::MThreadPool(unsigned count_of_threads)
{
	auto max_count = std::thread::hardware_concurrency();
	if (max_count != 0 && (count_of_threads == 0 || count_of_threads > max_count))
	{
		count_of_threads = max_count;
	}
	else if (max_count = 0 && count_of_threads == 0)
	{
		count_of_threads = 2;
	}
	if (!count_of_threads) throw std::invalid_argument("thread pool construction failed");
	m_threads.reserve(count_of_threads);
	for (unsigned i = 0; i < count_of_threads; ++i)
	{
		m_threads.emplace_back(&MThreadPool::RunExecution, this);
	}
	Log("Threads constructed.");
}

// Now my ThreadPool waits for threads before destruction BUT :
// Can i somehow remove that mutex ?
// What's better simple cycle with sleep or fake mutex ?
MThreadPool::~MThreadPool()
{
	Log("Started thread pool destruction !");

	std::mutex execution_finaliser;
	std::unique_lock<std::mutex> final_lock{ execution_finaliser };
	m_finish_indicator.wait(final_lock, [this] {return m_active_threads_count == 0; });

	std::unique_lock<std::mutex> lock{ m_queue_mutex };
	m_stop_flag = true;
	lock.unlock();

	m_notifier.notify_all();

	for (auto &thread : m_threads)
		thread.join();

	Log("Threads destructed.");
}

void MThreadPool::RunExecution()
{
	Log("Initialized.");
	bool initialized = false;
	while (true)
	{
		std::unique_lock<std::mutex> lock{ m_queue_mutex };
		if (initialized && m_available_tasks.empty() && m_active_threads_count == 0)
		{
			Log("All tasks done.");
			m_finish_indicator.notify_one();
		}
		initialized = true;

		m_notifier.wait(lock, [this] {return m_stop_flag || !m_available_tasks.empty(); });
		if (!m_stop_flag)
		{
			++m_active_threads_count;
			auto current_task = std::move(m_available_tasks.front());
			m_available_tasks.pop();
			lock.unlock();
			Log("Started task execution.");
			current_task();
			Log("Task finished.");
			--m_active_threads_count;
		}
		else return;
	}
}

void MThreadPool::Log(const std::string & message)
{
	std::lock_guard<std::mutex> lock{ m_log_mutex };

	std::cout << "Thread [" << std::this_thread::get_id() << "] - :" << message << std::endl;
}
