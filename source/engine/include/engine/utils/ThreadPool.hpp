#pragma once

#include "ThreadSafeQueue.hpp"

#include <atomic>
#include <future>
#include <memory>
#include <thread>
#include <vector>

class ThreadPool
{
private:
	class IThreadTask
	{
	public:
		IThreadTask() = default;

		virtual ~IThreadTask() = default;

		IThreadTask(IThreadTask const& rhs) = delete;

		IThreadTask& operator=(IThreadTask const& rhs) = delete;

		IThreadTask(IThreadTask&& other) = default;

		IThreadTask& operator=(IThreadTask&& other) = default;

		/**
		 * Run the task.
		 */
		virtual void execute() = 0;
	};

	template<typename Func>
	class ThreadTask : public IThreadTask
	{
	public:
		ThreadTask(Func&& func) : m_func{std::move(func)}
		{ }

		~ThreadTask() override = default;

		ThreadTask(ThreadTask const& rhs) = delete;

		ThreadTask& operator=(ThreadTask const& rhs) = delete;

		ThreadTask(ThreadTask&& other) = default;

		ThreadTask& operator=(ThreadTask&& other) = default;

		/**
		 * Run the task.
		 */
		void execute() override
		{
			m_func();
		}

	private:
		Func m_func;
	};

public:
	/**
	 * A wrapper around a std::future that adds the behavior of futures returned from std::async.
	 * Specifically, this object will block and wait for execution to finish before going out of scope.
	 */
	template<typename T>
	class TaskFuture
	{
	public:
		TaskFuture(std::future<T>&& future) : m_future{std::move(future)}
		{ }

		TaskFuture(TaskFuture const& rhs) = delete;

		TaskFuture& operator=(TaskFuture const& rhs) = delete;

		TaskFuture(TaskFuture&& other) = default;

		TaskFuture& operator=(TaskFuture&& other) = default;

		~TaskFuture(void)
		{
			if (m_future.valid())
				m_future.get();
		}

		auto get(void)
		{
			return m_future.get();
		}

	private:
		std::future<T> m_future;
	};

public:
	/**
	 * Constructor.
	 */
	ThreadPool() : ThreadPool{std::max(1u, std::thread::hardware_concurrency() - 1u)}
	{ }

	/**
	 * Constructor.
	 */
	explicit ThreadPool(uint32_t const numThreads) : _done{false}, _workQueue{}, _threads{}
	{
		try
		{
			for (uint32_t i = 0u; i < numThreads; ++i)
				_threads.emplace_back(&ThreadPool::worker, this);
		}
		catch (...)
		{
			_done = true;
			throw;
		}
	}

	/**
	 * Non-copyable.
	 */
	ThreadPool(ThreadPool const& rhs) = delete;

	/**
	 * Non-assignable.
	 */
	ThreadPool& operator=(ThreadPool const& rhs) = delete;

	/**
	 * Destructor.
	 */
	~ThreadPool()
	{
		_done = true;
		_workQueue.invalidate();
		for (auto& thread : _threads)
			if (thread.joinable())
				thread.join();
	}

	/**
	 * Submit a job to be run by the thread pool.
	 */
	template<typename Func, typename... Args>
	auto submit(Func func, Args&&... args)
	{
		auto boundTask = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
		using ResultType = std::result_of_t<decltype(boundTask)()>;
		using PackagedTask = std::packaged_task<ResultType()>;
		using TaskType = ThreadTask<PackagedTask>;

		PackagedTask task{std::move(boundTask)};
		TaskFuture<ResultType> result{task.get_future()};
		_workQueue.push(std::make_unique<TaskType>(std::move(task)));
		return result;
	}

private:
	/**
	 * Constantly running function each thread uses to acquire work items from the queue.
	 */
	void worker()
	{
		while (!_done)
		{
			std::unique_ptr<IThreadTask> pTask{nullptr};
			if (_workQueue.wait_pop(pTask))
				pTask->execute();
		}
	}

private:
	std::atomic_bool _done;
	ThreadSafeQueue<std::unique_ptr<IThreadTask>> _workQueue;
	std::vector<std::thread> _threads;
};
