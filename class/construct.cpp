#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
using namespace std;

class A {
public:
	int num1, num2;
	explicit A() :num1(0), num2(0) { cout << "simple" << endl; }
	A(int n1, int n2) : num1(n1), num2(n2) { cout << "值传递构造" << endl; }
	A(const A& a) {
		num1 = a.num1;
		num2 = a.num2;
		cout << "copy constructor" << endl;
	}
	A(A&& a) {
		num1 = a.num1;
		num2 = a.num2;
		cout << "move constructor" << endl;
		move(a);
	}
	A& operator=(const A& a) {
		cout << "=操作符，对象引用" << endl;
		num1 = a.num1 + 1;
		num2 = a.num2 + 1;
		return *this;
	}
	A& operator=(vector<int>& nums) {
		cout << "=操作符，值传递" << endl;
		num1 = nums[0];
		num2 = nums[1];
		return *this;
	}
};

int main() {

	A a1;
	A a2(1, 3); //构造函数
	A a3 = { 2, 3 }; //构造函数 和上面能一样,不是拷贝构造 当初始化构造函数定义为显式的时候，这个直接使用A a3 = {2,3}就不可行
	A a3_(A( 2, 4 )); // 直接将临时对象 A(2, 4) 构造成了 a3_，而没有额外的拷贝操作。 a2,a3,a3_都是初始化构造
	cout << a3_.num2 << endl;
	cout << "----------" << endl;
	//拷贝构造
	A a_4(a2); //拷贝构造
	A a4 = a2; //拷贝构造函数 拷贝构造函数定义为显式的时候就不能这样只能A a4(a2);
	cout << "--------" << endl;
	A a5; //默认构造函数
	a5 = a2; //=重载构造
	
	a5 = { 1,7 }; //=重载构造 ，先{1,7} 调用构造函数，再拷贝构造
	A a6;
	vector<int> n = { 1,7 };
	a6 = n;
	A a7(move(a2));
}