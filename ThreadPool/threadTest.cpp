#include"myThreadPool.h"
#include"TaskQueue.h"
#include<vld.h>
void taskFunc(void* arg) {
	int num = *(int*)arg;
	std::cout << "current id is: " << std::this_thread::get_id() << " :" << num << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));

}

void test() {
	std::cout << "Test my threadPool!" << std::endl;
	ThreadPool myThreadPool(2, 10);
	for (int i = 0; i < 100; i++) {
		int* num = new int(i);
		myThreadPool.addTask(Task{ taskFunc, num });

	}
	std::this_thread::sleep_for(std::chrono::seconds(20));
	std::cout << "Test ended." << std::endl;
}

int main() {
	test();

	
}