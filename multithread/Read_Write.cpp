#include<iostream>
#include<mutex>
#include<memory>
#include<thread>
#include<map>
#include<shared_mutex>
#include<condition_variable>
class dns_entry {
public:
	std::string name;
	int id;
	dns_entry() :name("null"), id(-1) {}
	dns_entry(std::string name_, int id_) :name(name_), id(id_) {}

};
class dns_cache {
private:
	mutable std::condition_variable cond;
	std::map<std::string, dns_entry> entries;
	mutable std::shared_mutex entry_mutex;   //����������Ψһ д��ռ �����ڶ�����д��������д���ɶ�
	mutable std::mutex condition_mutex;
public:
	dns_entry find_entry(std::string const& domain) const {
		//ʹ��cond.wait() ʱ������Ҫһ������Ļ����������������ļ�顣�������Ļ��������� entry_mutex������һ�������Ļ�������
		std::unique_lock<std::mutex> cond_lock(condition_mutex); //��������ͷţ��������̷߳����ٽ��� �����������˳�����̰߳�ȫ��
		cond.wait(cond_lock, [&] {return !entries.empty(); });

		std::shared_lock<std::shared_mutex> lk(entry_mutex);
		
		auto it = entries.find(domain);	
		return (it == entries.end()) ? dns_entry() : it->second;
	}
	void write(std::string const& domain, dns_entry const& dns_entries)
	{	
		std::unique_lock<std::mutex> cond_lock(condition_mutex);
		std::lock_guard<std::shared_mutex> lk(entry_mutex);
		entries[domain] = dns_entries;
		cond.notify_one();
	}
};


void test1(std::shared_ptr<dns_cache>& p, const std::string& domain,const dns_entry& dns_entries) {
	p->write(domain, dns_entries);
}

void test2(std::shared_ptr<dns_cache>& p, const std::string& domain) {
	auto d = p->find_entry(domain);
	std::cout << d.id << " " << d.name << std::endl;
}
int main() {
	std::shared_ptr<dns_cache> p1 = std::make_shared<dns_cache>();
	std::shared_ptr<dns_cache> p4(p1); 
	std::shared_ptr<dns_cache> p2(p1);
	std::shared_ptr<dns_cache> p3(p1);
	std::thread t1(test1, ref(p1),"1", dns_entry("hzh", 111));  //����������ܱ�֤��д�߽�����Ⱥ�ֻ��ȷ����һ���������д�ߣ�ֻ��д��д�����ݶ��߲��ܶ�
	std::thread t4(test1, ref(p4), "2", dns_entry("hzh", 222));
	std::thread t2(test2, ref(p2), "1");
	std::thread t3(test2, ref(p3), "2");
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	//std::cout << p2.use_count();
}