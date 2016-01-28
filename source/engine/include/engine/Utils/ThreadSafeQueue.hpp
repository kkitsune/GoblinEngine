#pragma once

#include <condition_variable>
#include <atomic>
#include <mutex>
#include <queue>

template<typename T>
class ThreadSafeQueue
{
public:
	/**
	 * Destructor.
	 */
	~ThreadSafeQueue()
	{
		invalidate();
	}

	/**
	 * Attempt to get the first value in the queue.
	 * Returns true if a value was successfully written to the out parameter, false otherwise.
	 */
	bool try_pop(T& out)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_queue.empty() || !_valid)
			return false;
		out = std::move(_queue.front());
		_queue.pop();
		return true;
	}

	/**
	 * Get the first value in the queue.
	 * Will block until a value is available unless clear is called or the instance is destructed.
	 * Returns true if a value was successfully written to the out parameter, false otherwise.
	 */
	bool wait_pop(T& out)
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_condition.wait(lock, [this]()
		{
			return !_queue.empty() || !_valid;
		});
		/*
		 * Using the condition in the predicate ensures that spurious wakeups with a valid
		 * but empty queue will not proceed, so only need to check for validity before proceeding.
		 */
		if (!_valid) return false;
		out = std::move(_queue.front());
		_queue.pop();
		return true;
	}

	/**
	 * Push a new value onto the queue.
	 */
	void push(T value)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_queue.push(std::move(value));
		_condition.notify_one();
	}

	/**
	 * Check whether or not the queue is empty.
	 */
	bool empty() const
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return _queue.empty();
	}

	/**
	 * Clear all items from the queue.
	 */
	void clear()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		while (!_queue.empty())
			_queue.pop();
		_condition.notify_all();
	}

	/**
	 * Invalidate the queue.
	 * Used to ensure no conditions are being waited on in wait_pop when
	 * a thread or the application is trying to exit.
	 * The queue is invalid after calling this method and it is an error
	 * to continue using a queue after this method has been called.
	 */
	void invalidate()
	{
		std::lock_guard<std::mutex> lock{_mutex};
		_valid = false;
		_condition.notify_all();
	}

	/**
	 * Returns whether or not this queue is valid.
	 */
	bool is_valid() const
	{
		std::lock_guard<std::mutex> lock{_mutex};
		return _valid;
	}

private:
	std::atomic_bool _valid{true};
	mutable std::mutex _mutex;
	std::queue<T> _queue;
	std::condition_variable _condition;
};
