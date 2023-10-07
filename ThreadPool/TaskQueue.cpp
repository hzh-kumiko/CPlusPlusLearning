#include "TaskQueue.h"


Task::Task() :function(nullptr), arg(nullptr) {}
Task::Task(callback f_, void* arg_) : function(f_), arg(arg_) {}


TaskQueue::TaskQueue()
{
	
}

TaskQueue::~TaskQueue()
{

}

void TaskQueue::addTask(Task task)
{
	std::lock_guard<std::mutex> myLock(m_mutex);
	m_TaskQ.push(task);
}

void TaskQueue::addTask(callback f, void* arg)
{
	std::lock_guard<std::mutex> myLock(m_mutex);
	m_TaskQ.push(Task{ f, arg });
}

Task TaskQueue::takeTask()
{
	Task t;
	std::lock_guard<std::mutex> myLock(m_mutex);
	if (!m_TaskQ.empty()) {
		
		t = m_TaskQ.front();
		m_TaskQ.pop();
	}
	

	return t;
}

int TaskQueue::getTaskNumber()
{
	std::lock_guard<std::mutex> myLock(m_mutex);
	return m_TaskQ.size();
}
