#include<iostream>
#include<mutex>

class Hierarchical_Mutex
{
public:
	explicit Hierarchical_Mutex(unsigned long value) : hierarchical_value(value), prev_hierarchical_value(0) {}
	~Hierarchical_Mutex() {}
	void lock() {
		check();
		internal_mutex.lock();
		update();
	}
	void unlock() {
		this_thread_hierarchical_value = prev_hierarchical_value; //�ص���һ����
		internal_mutex.unlock();
		
	}
	bool try_lock() {
		check();
		if (!internal_mutex.try_lock()) return false;
		update();
		return true;
	}
private:
	std::mutex internal_mutex;
	const unsigned long hierarchical_value; //��ǰ�Ĳ㼶
	unsigned long prev_hierarchical_value;
	static thread_local unsigned long this_thread_hierarchical_value;

	void check() {
		//��ǰ�̵߳Ĳ㼶ֵ�ȸ����Ĳ㼶ֵҪС�Ļ�����ȡ����ʧ��
		if (this_thread_hierarchical_value <= hierarchical_value) {   
			throw std::logic_error("mutex hierarchy violated");
		}
	}

	void update() {
		prev_hierarchical_value = this_thread_hierarchical_value;
		this_thread_hierarchical_value = hierarchical_value;
	}
	

};

thread_local unsigned long Hierarchical_Mutex::this_thread_hierarchical_value = ULONG_MAX;




int main() {
	Hierarchical_Mutex hM(1000);
	Hierarchical_Mutex lM(100);

	{
		std::scoped_lock lk(hM, lM);
		std::cout << "A hierarchical mutex is established" << std::endl;
	}

	{
		std::scoped_lock lk(lM, hM);
		//std::cout << "A hierarchical mutex is established" << std::endl;
	}

	
}