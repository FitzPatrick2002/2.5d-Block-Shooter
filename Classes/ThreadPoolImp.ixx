import ThreadPool;


ThreadPool::ThreadPool() : total_threads_num(std::thread::hardware_concurrency()) {
	if (total_threads_num == 0) {
		total_threads_num = 1; // Ensure at least one thread
	}

}

ThreadPool::~ThreadPool() {
	shutdown(); // Call shutdown in the destructor
}

// Thread pool is treated as a singleton 
ThreadPool& ThreadPool::accessPool() {
	static ThreadPool pool;

	return pool;
}

// Start the threadpool. Creates a specified number of threads.
void ThreadPool::start() {
	if (!threads.empty()) {
		// Already started
		return;
	}
	threads.reserve(total_threads_num);
	for (int i = 0; i < total_threads_num; ++i) { // Use total_threads_num for clarity
		threads.emplace_back([this] { this->worker_thread(); });
	}
}

// Shutdown the threadpool
void ThreadPool::shutdown() {
	{
		std::lock_guard<std::mutex> lock(mutex);
		shutdown_request = true;
	}
	cv.notify_all(); // Notify all threads to wake up and check the shutdown flag

	// Wait till each thread finishes their job.
	// Join threads to the main thread.
	for (auto& t : threads) {
		if (t.joinable()) {
			t.join();
		}
	}
	threads.clear(); // Clear the thread vector after joining
}

// Function realised by each worker thread.
// 
void ThreadPool::worker_thread() {
	std::unique_lock<std::mutex> lock(mutex);

	while (true) {
		// Wait until shutdown is requested or there is a task in the queue
		cv.wait(lock, [this] {
			return shutdown_request || !queue.empty();
			});

		// Check for shutdown request after waking up.
		// If shutdown is requested and the queue is empty, exit the thread.
		if (shutdown_request && queue.empty()) {
			break;
		}

		// If we woke up and there's a task
		if (!queue.empty()) {
			busy_threads++; // Increment busy count after taking a task
			auto func = queue.front();
			queue.pop();

			// Unlock the mutex while executing the task
			lock.unlock();

			func(); // Execute the task

			// Re-lock the mutex before checking the queue or waiting again
			lock.lock();
			busy_threads--; // Decrement busy count after completing the task
		}
	} // Loop continues until shutdown_request is true and queue is empty
}

template <typename F, typename... Args>
auto ThreadPool::queueTask(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {

	auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
	auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

	auto wrapper_function = [task_ptr]() { (*task_ptr)(); };

	{
		std::lock_guard<std::mutex> lock(mutex);
		if (shutdown_request) {
			throw std::runtime_error("queueTask on stopped ThreadPool");
		}
		queue.push(wrapper_function);
	} // Lock is released 

	cv.notify_one(); // Notify one waiting thread that a task is available

	return task_ptr->get_future();
}

// Tells if any thread is working
bool ThreadPool::anyThreadWorking() const { 
	return busy_threads.load() > 0; 
}

// Returns number of currently busy threads
int ThreadPool::getBusyThreads() const { 
	return busy_threads.load(); 
}

// Get the total number of threads
int ThreadPool::getTotalThreads() const { 
	return total_threads_num;
}

// Returns number of tasks queued in the queue
int ThreadPool::getQueueLength() const {
	return queue.size();
}