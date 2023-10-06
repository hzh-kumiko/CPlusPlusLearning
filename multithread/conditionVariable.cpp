#include<iostream>
#include<condition_variable>
#include<queue>
#include<thread>
#include<Windows.h>
std::mutex mut;
std::queue<int> data;
std::condition_variable data_cond;

void data_preparation_thread() {
	int i = 0;
	while (i < 10) {
		std::lock_guard<std::mutex> mylock(mut);
		data.push(i);
		i++;
		data_cond.notify_one();
		Sleep(100);
	}
}

void data_processing_thread() {
	while (true) {
		std::unique_lock<std::mutex> mylock(mut);
		data_cond.wait(mylock, [] {return !data.empty(); });
		int da = data.front();
		std::cout << data.front()<<data.back()<<std::endl;
		data.pop();
		mylock.unlock();
		if (data.empty()) {
			break;
		}
	}
}
int main() {
	std::thread t1(data_preparation_thread);
	std::thread t2(data_processing_thread);
	t1.join();
	t2.join();
	
}