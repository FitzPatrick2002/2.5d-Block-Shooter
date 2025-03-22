export module ThreadPool;

import <queue>;
import <thread>;
import <mutex>;
import <functional>;
import <vector>;
import <utility>;

export class ThreadPool {
private:

	void threadLoop();

	bool shouldTerminate = false;
	std::mutex queueMutex;
	std::condition_variable mutexCondition;
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> jobs;

public:

	ThreadPool() = default;
	~ThreadPool() = default;

	void start();

	template<typename Func, typename ...Args>
	void queueJob(Func&& func, Args &&...args) {
		{
			std::unique_lock<std::mutex> lock(this->queueMutex);
			jobs.push([func = std::forward<Func>(func), ...args = std::forward<Args>(args)] {
				std::invoke(func, args...);
				});
		}

		this->mutexCondition.notify_one();
	}
	
	void stop();
	bool busy();
};