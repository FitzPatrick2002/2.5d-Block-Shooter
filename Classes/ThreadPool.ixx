export module ThreadPool;

import <future>;
import <queue>;
import <thread>;
import <mutex>;
import <functional>;
import <vector>;
import <utility>;

// Singleton
// Threadpool contains a vector of started threads.
// They are halted until a new task is queued, then one of the threads is notified and takes up the job. Task is removed from the queue.

export class ThreadPool {
private:
	mutable std::mutex mutex;
	std::condition_variable cv;

	std::vector<std::thread> threads;
	bool shutdown_request = false; // Initialize shutdown_request

	std::queue<std::function<void()>> queue;

	int total_threads_num;
	std::atomic<int> busy_threads{ 0 }; // Use atomic for thread-safe operations

public:
	ThreadPool();
	

	~ThreadPool();

void start();

	// New shutdown method
void shutdown();

void worker_thread();

	template <typename F, typename... Args>
	auto queueTask(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;

	static ThreadPool& accessPool();

	bool anyThreadWorking() const;

	int getBusyThreads() const;

	int getTotalThreads() const;

	int getQueueLength() const;
};