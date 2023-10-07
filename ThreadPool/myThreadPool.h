#pragma once
#include "TaskQueue.h"
#include <vector>
#include <condition_variable>
#include <chrono>
class ThreadPool {
private:
	TaskQueue* m_Task;
	std::vector<std::thread> workThreads;
	std::thread managerThread;

	int minNum;
	int maxNum;
	int busyNum;
	int liveNum;
	int exitNum;
	std::mutex mutexPool;         //锁线程池
	std::condition_variable cond; //任务队列非空才调度线程

	bool shutdown;                 //是否要销毁线程池

public:
	ThreadPool(int min_, int max_);
	~ThreadPool();
	void addTask(Task task);
	int getBusyNumber();
	int getAliveNumber();
	void waitAllTasks() {
		while (!m_Task->getTaskNumber() != 0 || busyNum > 0) {
			std::this_thread::sleep_for(std::chrono::seconds(20));
		}
	}
private:
	static void worker(void* arg); //工作线程
	static void manager(void* arg);
	//void threadExit();
	static const int NUMBER = 2;
};