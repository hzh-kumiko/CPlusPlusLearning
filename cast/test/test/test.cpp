#include<iostream>
#include<vector>
#include <typeinfo>
using namespace std;
class A
{
public:
	A() {}
	void foo()
	{
		cout << "A!" << endl;
	}
	static void print() {
		cout << "A print!" << endl;
	}
};
class B :public A
{
	char b = 'c';
public:
	B() {}
	void foo()
	{
		cout << b << endl;
	}
	void print() {
		cout << "B print!" << endl;
	}
};

int main() {
	/* reinterpret_cast
	reinterpret_cast是四种强制转换中功能最为强大的（最暴力，最底层，最不安全）。它的本质是编译器的指令。
	它的作用：它可以把一个指针转换成一个整数，也可以把一个整数转换成一个指针。或者不同类型的指针的相互替换
	
	double a = 1.1;
	char* c = reinterpret_cast<char*>(&a);
	double* b = reinterpret_cast<double*>(c);
	cout << *b << endl;*/

	/* const_cast
	* 有两个功能，去掉const和加上const
	*/
	//const int a = 1;
	//int* p;
	//p = const_cast<int*>(&a);
	//(*p)++;
	int i = 3;
	const int a =1;
	int& r = const_cast<int&>(a);
	r++;
	cout << i << endl;
	cout << a << endl;
	cout << r << endl;
	cout << &a <<" "<< &r << endl;
	
	
	/* static_cast	
	该运算符把 expression 转换为 type-id 类型，但没有运行时类型检查来保证转换的安全性。它主要有如下几种用法：

① 用于类层次结构中基类和子类之间指针或引用的转换。进行上行转换（把子类的指针或引用转换成基类表示）是安全的；进行下行转换（把基类指针或引用转换成子类表示）时，由于没有动态类型检查，所以是不安全的。
② 用于基本数据类型之间的转换，如把 int 转换成 char，把 int 转换成 enum。这种转换的安全性也要开发人员来保证。
③ 把空指针转换成目标类型的空指针。
④ 把任何类型的表达式转换成void类?*/
	double s = 1.1;
	int si = static_cast<int>(s);
	cout << si << endl;
	double s2 = static_cast<double>(si+0.1);
	cout << s2 << endl;
	A* aa = new B();
	aa->print();
	B* bb = new B();
	bb->print();
	static_cast<B*>(aa)->foo();
	/*
	dynamic_cast（动态转换），允许在运行时刻进行类型转换，从而使程序能够在一个类层次结构安全地转换类型。
	dynamic_cast 提供了两种转换方式，把基类指针转换成派生类指针，或者把指向基类的左值转换成派生类的引用。
	*/
	return 0;
}