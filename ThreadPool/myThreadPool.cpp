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
	//�������չ�����
	if (managerThread.joinable()) managerThread.join();
	cond.notify_all(); //�������е������߳�
	for (int i = 0; i < maxNum; i++) {
		if (workThreads[i].joinable()) workThreads[i].join();
	}
	if (m_Task) {
		delete m_Task;
	}
	
}

void ThreadPool::addTask(Task task)
{
	//m_Task��addTask�Ѿ�������
	if (shutdown) return;
	m_Task->addTask(task);
	//����notify����Ϊwaitֻ����û�����������£����������߳̾ͻ�����while

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
		//����̳߳�δ�ر���û����������Կ����Ƿ�Ҫ���ٲ����߳�
		while (pool->m_Task->getTaskNumber() == 0 && !pool->shutdown) {
			pool->cond.wait(myLock); //�������ȴ������ѣ�
			//�Ƿ�Ҫ�����߳� 
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

		//�Ƿ�ر��̳߳�
		if (pool->shutdown) {
			myLock.unlock();
			std::cout << "thread" << std::this_thread::get_id()<< " is shutdown." << std::endl;
			return;
		}

		//��������û�б�shutdown��ȡ������
		Task t = std::move(pool->m_Task->takeTask());
		pool->busyNum++;
		//�����ţ����Խ���
		myLock.unlock();

		std::cout << "thread" << std::this_thread::get_id() << " is working" << std::endl;
		t.function(t.arg);
		delete t.arg;
		t.arg = nullptr;
		std::cout << "thread" << std::this_thread::get_id() << " stoped working" << std::endl;
		
		//�̹߳����������޸�busynum������
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

		//����߳�
		//�������ĸ��� > ����̵߳ĸ��� && ����̵߳ĸ��� < ����߳���
		if (queueSize > liveNum && liveNum < pool->maxNum) {
			std::unique_lock<std::mutex> myLock(pool->mutexPool);
			int count = 0;
			for (int i = 0; i < pool->maxNum && count < NUMBER && pool->liveNum < pool->maxNum; i++) {
				//�ж��߳��Ƿ�δ��ʼ�����ͷ���
				if (pool->workThreads[i].get_id() == std::thread::id()) {
					std::cout << "Create a new thread." << std::endl;
					pool->workThreads[i] = std::thread(worker, pool);
					count++;
					pool->liveNum++;
				}
			}
		}
		//�����߳�̫�࣬����һ����
		if (busyNum * 2 < liveNum && liveNum > pool->minNum) {
			std::unique_lock<std::mutex> myLock(pool->mutexPool);
			pool->exitNum = NUMBER;
			for (int i = 0; i < NUMBER; i++) {
				pool->cond.notify_one();
			}
		}
	}

}
