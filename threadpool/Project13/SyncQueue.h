#pragma once
#include<iostream>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<list>
template<typename T>
class SyncQueue
{
private:
	std::list<T>m_queue;
	std::mutex m_mutex;
	std::condition_variable m_notEmpty;
	std::condition_variable m_notFull;
	int m_maxSize;
	bool m_needStop;
private:
	bool NotEmpty() const
	{
		bool empty = m_queue.empty();
		if (empty)
		{
			std::cout << "缓存区满了，需要等待异步线程ID：" << std::this_thread::get_id() << std::endl;
		}
		return !empty;
	}
	bool NotFull() const
	{
		bool full = m_queue.size() >= m_maxSize;
		if (full)
		{
			std::cout << "缓存区满了，需要等待。。。" << std::endl;
		}
		return !full;
	}
	template<typename F>
	void Add(F&& f)
	{
		std::unique_lock<std::mutex>locker(m_mutex);
		m_notFull.wait(locker, [this] {return m_needStop || NotFull(); });
		if (m_needStop)
		{
			return;
		}
		m_queue.push_back(std::forward<F>(f));
		m_notEmpty.notify_one();
	}
public:
	SyncQueue(int maxSize) :m_maxSize(maxSize), m_needStop(false) {}
	void Put(const T& x)
	{
		Add(x);
	}
	void Put(T&& x)
	{
		Add(std::forward<T>(x));
	}
	bool Empty()
	{
		std::unique_lock<std::mutex>locker(m_mutex);
		return m_queue.empty();
	}
	bool Full()
	{
		std::unique_lock<std::mutex>locker(m_mutex);
		return m_queue.size() == m_maxSize;
	}
	size_t Size()
	{
		std::unique_lock<std::mutex>locker(m_mutex);
		return m_queue.size();
	}
	int Count()
	{
		return m_queue.size();
	}
	void Stop()
	{
		{
			std::unique_lock<std::mutex>locker(m_mutex);
			m_needStop = true;
		}
		m_notEmpty.notify_all();
		m_notFull.notify_all();
	}
	void Take(std::list<T>& list)
	{
		std::unique_lock<std::mutex>locker(m_mutex);
		m_notEmpty.wait(locker, [this] {return m_needStop || NotEmpty(); });
		if (m_needStop)
		{
			return;
		}
		list = std::move(m_queue);
		m_notFull.notify_one();
	}
	void Take(T& t)
	{
		std::unique_lock<std::mutex>locker(m_mutex);
		m_notEmpty.wait(locker, [this] {return m_needStop || NotEmpty(); });
		if (m_needStop)
		{
			return;
		}
		t = m_queue.front();
		m_queue.pop_front();
		m_notFull.notify_one();
	}
};