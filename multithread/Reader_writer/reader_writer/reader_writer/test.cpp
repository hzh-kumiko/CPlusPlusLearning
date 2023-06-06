#include<iostream>
#include<mutex>
#include<memory>
/*
#include <boost/thread.hpp>    
#include <boost/thread/mutex.hpp>  
class dns_entry {};
class dns_cache {
private:
	std::map<std::string, dns_entry> entries;
	mutable boost::shared_mutex entry_mutex;   //共享锁读不唯一 写独占 有人在读则不能写，有人在写不可读
public:
	dns_entry find_entry(std::string const& domain) const {
		boost::shared_lock<boost::shared_mutex> lk(entry_mutex);
		auto it = entries.find(domain);
		return (it == entries.end()) ? dns_entry() : it->second;
	}
	void write(std::string const& domain, dns_entry const& dns_entries)
	{
		std::lock_guard<boost::shared_mutex> lk(entry_mutex);
		entries[domain] = dns_entries;
	}
};
*/

void test(std::shared_ptr<int>& p) {
	std::cout << p.use_count();
}
int main() {
	
	
	char c[5] = { '1','\0','3' };
	printf("%s", c);
	//std::cout << p2.use_count();
}