#pragma once

#include<iostream>
#include<queue>
#include<mutex>
#include<thread>
typedef void(*callback)(void*);
 
struct Task {
	Task();
	Task(callback f_, void* arg_);
	callback function;
	void* arg;
};

class TaskQueue {
private:
	std::queue<Task> m_TaskQ;
	mutable std::mutex m_mutex;
public:
	TaskQueue();
	~TaskQueue();
	//Add Task
	void addTask(Task task);
	void addTask(callback f, void* arg);
	//Get Task
	Task takeTask();
	//Get the number of current tasks
	int getTaskNumber();
};
