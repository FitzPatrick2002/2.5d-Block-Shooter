import ThreadPool;

void ThreadPool::start() {
	const uint32_t threadsNum = std::thread::hardware_concurrency();

	for (uint32_t i = 0; i < threadsNum; i++)
		this->threads.emplace_back(std::thread(&ThreadPool::threadLoop, this));
}

void ThreadPool::stop() {
	{
		std::unique_lock<std::mutex> lock(this->queueMutex);
		this->shouldTerminate = true;
	}
	
	this->mutexCondition.notify_all();

	for (std::thread& activeThread : this->threads)
		activeThread.join();

	this->threads.clear();
}

bool ThreadPool::busy() {
	bool poolBusy;
	{
		std::unique_lock<std::mutex> lock(this->queueMutex);
		poolBusy = not jobs.empty();
	}
	return poolBusy;
}

void ThreadPool::threadLoop() {
	while (true) {
		std::function<void()> job;
		{
			std::unique_lock<std::mutex> lock(this->queueMutex);
			this->mutexCondition.wait(lock, [this] {
				return not jobs.empty() || shouldTerminate;
				});

			if (shouldTerminate)
				return;

			job = jobs.front();
			jobs.pop();
		}

		job();
	}
}
