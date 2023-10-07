#include<iostream>
#include<thread>
#include<Windows.h>
#include<mutex>
#include<atomic>

using namespace std;
mutex myMutex1;
mutex myMutex2;
static bool s_Finished = false;
//atomic<int> i(0);  //原子操作，不用加锁
int i = 0;

void print() {
	cout << "print_thred_id = " << this_thread::get_id() << endl;
	while (!s_Finished) {
		cout << "print" << endl;
		this_thread::sleep_for(1s);

	}
}
void test() {
	//myMutex.lock();
	//lock_guard<mutex> lk(myMutex);  Q: 在外面加锁比在里面加锁时间少
	for (int k = 0; k < 1000000; k++) {
		lock(myMutex1, myMutex2);   //原子锁
		lock_guard<mutex> lg1(myMutex1, adopt_lock);
		lock_guard<mutex> lg2(myMutex2, adopt_lock);
		i++;
	}

	//myMutex.unlock();  //如果unlock 发生异常处理，抛出异常，则无法解锁， 另外线程也无法继续，引入lock_guard
}
void test1() {
	//myMutex.lock();
	//lock_guard<mutex> lk(myMutex);  Q: 在外面加锁比在里面加锁时间少
	for (int k = 0; k < 1000000; k++) {
		lock(myMutex1, myMutex2);
		lock_guard<mutex> lg1(myMutex2, adopt_lock);
		lock_guard<mutex> lg2(myMutex1, adopt_lock);    //两个锁顺序相反导致死锁，顺序相同可以解决
		i++;
	}

	//myMutex.unlock();  //如果unlock 发生异常处理，抛出异常，则无法解锁， 另外线程也无法继续，引入lock_guard
}
int main() {

	clock_t start = clock();
	int cnt = 100;
	thread MyThread1(test);

	thread MyThread2(test1);   //在传参的时候，值传递，引用传递地址都不同，指针传递地址相同，不用指针传递，加上ref()后为引用，地址相同，在类对象传递时，用显示传递
										//线程创建时如果没有ref则会新拷贝一个类，调用一次拷贝构造函数， 传递时如果不是引用传递再调用一次
	MyThread1.join();
	MyThread2.join();
	cout << i << endl;
	clock_t end = clock();
	cout << "用时：" << (double)(end - start) << endl;

	thread Pthread(print);
	cin.get();
	s_Finished = true;
	Pthread.join();
	cout << "finished" << endl;
	cout << "main_thred_id = " << this_thread::get_id() << endl;

	return 0;
}