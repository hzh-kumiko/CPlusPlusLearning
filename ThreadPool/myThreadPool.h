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
	std::mutex mutexPool;         //���̳߳�
	std::condition_variable cond; //������зǿղŵ����߳�

	bool shutdown;                 //�Ƿ�Ҫ�����̳߳�

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
	static void worker(void* arg); //�����߳�
	static void manager(void* arg);
	//void threadExit();
	static const int NUMBER = 2;
};