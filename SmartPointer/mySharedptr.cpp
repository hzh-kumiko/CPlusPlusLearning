#include<iostream>
#include<memory>
template<class T>
class myShared_ptr {
private:
	T* _ptr; //指向数据
	int* _pcount; //指向计数
public:
	myShared_ptr(T* ptr = nullptr) :_ptr(ptr), _pcount(new int(1)) {
		std::cout << "constructor" << std::endl;
	}

	myShared_ptr(const myShared_ptr& s) :_ptr(s._ptr), _pcount(s._pcount) {
		(*_pcount)++;
		std::cout << "copy constructor" << std::endl;
	}
	myShared_ptr(myShared_ptr&& s) noexcept : _ptr(s._ptr), _pcount(s._pcount) {
		s._ptr = nullptr; // 将原始指针置为空
		s._pcount = nullptr; // 将计数指针置为空
	}

	~myShared_ptr() {
		if (_pcount != nullptr && --(*_pcount) == 0) {
			delete _ptr;
			delete _pcount;
			std::cout << "count is 0" << std::endl;
		}
		_ptr = nullptr;
		_pcount = nullptr;
		std::cout << "destructor" << std::endl;
	}


	myShared_ptr<T>& operator=(const myShared_ptr& s) {
		if (this != &s) {
			if (--(*_pcount) == 0) {
				delete _ptr;
				delete _pcount;
			}
			_ptr = s._ptr;
			_pcount = s._pcount;
			(*_pcount)++;
		}

		std::cout << "opertor constructor" << std::endl;
		return *this;
	}

	myShared_ptr<T>& operator=(myShared_ptr&& s) noexcept {
		if (this != &s) {
			if (*_pcount == 1) {
				delete _ptr;
				delete _pcount;
			}
			_ptr = s._ptr;
			_pcount = s._pcount;
			s._ptr = nullptr;
			s._pcount = nullptr;
		}
		std::cout << "move operator" << std::endl;
		return *this;
	}

	T& operator*() const {
		return *(this->_ptr);
	}

	T* operator->() const {
		return this->_ptr;
	}
	int use_count() const {
		return *this->_pcount;
	}

};

class A {
public:
	A() {}
	A(int a_) :a(a_) {

	}
	A(A&& a_) {
		a = a_.a;
	}
	void print() {

		std::cout << "A" << std::endl;
	}
private:
	int a;
};
int main() {
	//标准shared_ptr
	std::shared_ptr<int> p1 = std::shared_ptr<int>(new int);
	std::cout << *p1 << std::endl;
	std::shared_ptr<int> p2;
	p2 = move(p1);
	std::cout << *p2 << p2.use_count() << std::endl;
	std::shared_ptr<int> p_move(std::move(p2));
	std::cout << p_move.use_count() << std::endl;
	std::shared_ptr<A> pa = std::shared_ptr<A>(new A(3));
	pa->print();
	//自己的
	//创建
	myShared_ptr<int> p(new int(1));
	std::cout << *p << " " << p.use_count() << std::endl;
	myShared_ptr<int> p_(p);
	std::cout << *p_ << " " << p_.use_count() << std::endl;

	//变指向
	myShared_ptr<int> p_new(new int(2));
	p = p_new;
	std::cout << p_.use_count() << std::endl;
	std::cout << "*p = " << *p << " " << "the address have " << p_new.use_count() << " pointers" << std::endl;
	myShared_ptr<int> p_new1(std::move(p_new));
	std::cout << "*p = " << *p << " " << "the address have " << p_new1.use_count() << " pointers" << std::endl;
	p = std::move(p_);

	//->重载
	myShared_ptr<A> p_A(new A(3));
	p_A->print();
	(*p_A).print();
	std::cout << "-----------" << std::endl;



}