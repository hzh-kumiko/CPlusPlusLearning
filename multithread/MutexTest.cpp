#include<iostream>
#include<thread>
#include<Windows.h>
#include<mutex>
#include<atomic>

using namespace std;
mutex myMutex1;
mutex myMutex2;
static bool s_Finished = false;
//atomic<int> i(0);  //ԭ�Ӳ��������ü���
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
	//lock_guard<mutex> lk(myMutex);  Q: ��������������������ʱ����
	for (int k = 0; k < 1000000; k++) {
		lock(myMutex1, myMutex2);   //ԭ����
		lock_guard<mutex> lg1(myMutex1, adopt_lock);
		lock_guard<mutex> lg2(myMutex2, adopt_lock);
		i++;
	}

	//myMutex.unlock();  //���unlock �����쳣�����׳��쳣�����޷������� �����߳�Ҳ�޷�����������lock_guard
}
void test1() {
	//myMutex.lock();
	//lock_guard<mutex> lk(myMutex);  Q: ��������������������ʱ����
	for (int k = 0; k < 1000000; k++) {
		lock(myMutex1, myMutex2);
		lock_guard<mutex> lg1(myMutex2, adopt_lock);
		lock_guard<mutex> lg2(myMutex1, adopt_lock);    //������˳���෴����������˳����ͬ���Խ��
		i++;
	}

	//myMutex.unlock();  //���unlock �����쳣�����׳��쳣�����޷������� �����߳�Ҳ�޷�����������lock_guard
}
int main() {

	clock_t start = clock();
	int cnt = 100;
	thread MyThread1(test);

	thread MyThread2(test1);   //�ڴ��ε�ʱ��ֵ���ݣ����ô��ݵ�ַ����ͬ��ָ�봫�ݵ�ַ��ͬ������ָ�봫�ݣ�����ref()��Ϊ���ã���ַ��ͬ��������󴫵�ʱ������ʾ����
										//�̴߳���ʱ���û��ref����¿���һ���࣬����һ�ο������캯���� ����ʱ����������ô����ٵ���һ��
	MyThread1.join();
	MyThread2.join();
	cout << i << endl;
	clock_t end = clock();
	cout << "��ʱ��" << (double)(end - start) << endl;

	thread Pthread(print);
	cin.get();
	s_Finished = true;
	Pthread.join();
	cout << "finished" << endl;
	cout << "main_thred_id = " << this_thread::get_id() << endl;

	return 0;
}