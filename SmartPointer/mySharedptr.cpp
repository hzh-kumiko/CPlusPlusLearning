#include<iostream>
#include<memory>
template<class T>
class myShared_ptr {
private:
	T* _ptr; //ָ������
	int* _pcount; //ָ�����
public:
	myShared_ptr(T* ptr = nullptr) :_ptr(ptr), _pcount(new int(1)) { 
		std::cout << "constructor" << std::endl; 
	}

	myShared_ptr(const myShared_ptr& s) :_ptr(s._ptr), _pcount(s._pcount) {
		(*_pcount)++;
		std::cout << "copy constructor" << std::endl;
	}

	~myShared_ptr() {
		(*_pcount)--;
		if (*_pcount == 0) {
			delete _ptr;
			delete _pcount;
			_ptr = nullptr;
			_pcount = nullptr;
			std::cout << "count is 0" << std::endl;
		}
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

	T& operator*() const{
		return *(this->_ptr);
	}

	T* operator->() const {
		return this->_ptr;
	}
	int use_count() const{
		return *this->_pcount;
	}

};

class A {
public:
	A(){}
	A(int a_):a(a_) {

	}
	void print() {

		std::cout << "A" << std::endl;
	}
private:
	int a;
};
int main() {
	//��׼shared_ptr
	std::shared_ptr<int> p1 = std::shared_ptr<int>(new int);
	std::cout << *p1 << std::endl;
	std::shared_ptr<A> pa = std::shared_ptr<A>(new A(3));
	pa->print();
	//�Լ���
	//����
	myShared_ptr<int> p(new int(1));
	std::cout << *p << " " << p.use_count() << std::endl;
	myShared_ptr<int> p_(p);
	std::cout << *p_ << " " << p_.use_count() << std::endl;

	//��ָ��
	myShared_ptr<int> p_new(new int(2));
	p = p_new;
	std::cout << p_.use_count() << std::endl;
	std::cout << "*p = " << *p << " " << "the address have " << p_new.use_count() << " pointers" << std::endl;

	//->����
	myShared_ptr<A> p_A(new A(3));
	p_A->print();
	(*p_A).print();
	std::cout << "-----------" << std::endl;

}