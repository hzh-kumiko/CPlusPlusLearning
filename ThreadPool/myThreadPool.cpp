#include "TaskQueue.h"
#include "myThreadPool.h"


ThreadPool::ThreadPool(int min_, int max_)
{
	
	m_Task = new TaskQueue;
	if (m_Task == nullptr) {
		std::cout << "the task queue failed to create." << std::endl;
		return;
	}
	workThreads.resize(max_);
	minNum = min_;
	maxNum = max_;
	busyNum = 0;
	liveNum = min_;
	exitNum = 0;
	shutdown = false;
	managerThread = std::thread(manager, this);
	for (int i = 0; i < min_; i++) {
		workThreads[i] = std::thread(worker, this);
	}
	
}

ThreadPool::~ThreadPool()
{
	shutdown = true;
	//阻塞回收管理者
	if (managerThread.joinable()) managerThread.join();
	cond.notify_all(); //唤醒所有的阻塞线程
	for (int i = 0; i < maxNum; i++) {
		if (workThreads[i].joinable()) workThreads[i].join();
	}
	if (m_Task) {
		delete m_Task;
	}
	
}

void ThreadPool::addTask(Task task)
{
	//m_Task的addTask已经有锁了
	if (shutdown) return;
	m_Task->addTask(task);
	//不用notify，因为wait只用在没有任务的情况下，有任务了线程就会跳出while

}

int ThreadPool::getBusyNumber()
{
	std::unique_lock<std::mutex> myLock(mutexPool);
	return this->busyNum;
}

int ThreadPool::getAliveNumber()
{
	std::unique_lock<std::mutex> myLock(mutexPool);
	return this->liveNum;
}


void ThreadPool::worker(void* arg)
{
	ThreadPool* pool = static_cast<ThreadPool*>(arg);
	while (true) {
		std::unique_lock<std::mutex> myLock(pool->mutexPool);
		//如果线程池未关闭且没有任务，则可以考虑是否要销毁部分线程
		while (pool->m_Task->getTaskNumber() == 0 && !pool->shutdown) {
			pool->cond.wait(myLock); //阻塞。等待被唤醒；
			//是否要销毁线程 
			if (pool->exitNum > 0) {
				pool->exitNum--;
				if (pool->liveNum > pool->minNum) {
					pool->liveNum--;
					myLock.unlock();
					std::cout << "thread" << std::this_thread::get_id() << " is about to destroying" << std::endl;
				}
				return;
			}
		}

		//是否关闭线程池
		if (pool->shutdown) {
			myLock.unlock();
			std::cout << "thread" << std::this_thread::get_id()<< " is shutdown." << std::endl;
			return;
		}

		//有任务且没有被shutdown，取出任务
		Task t = std::move(pool->m_Task->takeTask());
		pool->busyNum++;
		//不干扰，可以解锁
		myLock.unlock();

		std::cout << "thread" << std::this_thread::get_id() << " is working" << std::endl;
		t.function(t.arg);
		delete t.arg;
		t.arg = nullptr;
		std::cout << "thread" << std::this_thread::get_id() << " stoped working" << std::endl;
		
		//线程工作结束，修改busynum，加锁
		myLock.lock();
		pool->busyNum--;

	}

}

void ThreadPool::manager(void* arg)
{
	using namespace std::literals::chrono_literals;
	ThreadPool* pool = static_cast<ThreadPool*>(arg);
	while (!pool->shutdown) {
		std::this_thread::sleep_for(3s);

		std::unique_lock<std::mutex> myLock(pool->mutexPool);
		int queueSize = pool->m_Task->getTaskNumber();
		int liveNum = pool->liveNum;
		int busyNum = pool->busyNum;
		myLock.unlock();

		//添加线程
		//如果任务的个数 > 存活线程的个数 && 存活线程的个数 < 最大线程数
		if (queueSize > liveNum && liveNum < pool->maxNum) {
			std::unique_lock<std::mutex> myLock(pool->mutexPool);
			int count = 0;
			for (int i = 0; i < pool->maxNum && count < NUMBER && pool->liveNum < pool->maxNum; i++) {
				//判断线程是否还未初始化或被释放了
				if (pool->workThreads[i].get_id() == std::thread::id()) {
					std::cout << "Create a new thread." << std::endl;
					pool->workThreads[i] = std::thread(worker, pool);
					count++;
					pool->liveNum++;
				}
			}
		}
		//空闲线程太多，销毁一部分
		if (busyNum * 2 < liveNum && liveNum > pool->minNum) {
			std::unique_lock<std::mutex> myLock(pool->mutexPool);
			pool->exitNum = NUMBER;
			for (int i = 0; i < NUMBER; i++) {
				pool->cond.notify_one();
			}
		}
	}

}
