#include <iostream>
#include <vector>
#include<functional>

void print(int a) {
	std::cout << "value: " << a << std::endl;
}
//函数指针作传参
void ForEach(const std::vector<int>& values, void(*func)(int)){
	for (int value : values) {
		func(value);
	}
}

void ForEach1(const std::vector<int>& values, const std::function<void(int)>& func) {
	for (int value : values) {
		func(value);
	}
}

typedef void(*printFunction)(int);  //typedef函数指针
typedef long long ll;     //简单说就是替换

int main()
{
	ll a;
	a = 1e11;
	std::cout << a << std::endl;
	auto function = &print;
	auto function1 = print;  //就相当与上一行，做了隐式转换 这两种当print有重载的时候还是需要用常规的，也就是下面的一种形式来定义函数指针
	void(*test)(int);
	test = print;  //定义一个函数指针，指向的是print
	std::cout << function << std::endl;
	std::cout << function1 << std::endl;
	function(1);
	printFunction myFunction = print;  //和3行是一样的
	test(4);
	myFunction(5);

	std::vector<int> values = { 1,2,3,5,4};
	ForEach(values, print); //myFunction,print,test 都可以

	std::cout << std::endl;
	//lamda表达式做函数指针
	auto lamda = [](int value) {std::cout << value << std::endl; };
	ForEach(values, lamda);
	std::cout << std::endl;
	//假如我们要传递一个外部的数据，需要在[]内添加= 或 & 来表示值传递所有外部参数
	// 或引用传递， 值传递是拷贝， 引用不是
	//同时 = 和 & 不能同时包含同一个数， 比如 [=a,&a]不可以
	int a_pass = 1;

	auto lamda1 = [=](int value) {std::cout << value << std::endl; };
	//用了lamda1就不能用ForEach，因为ForEach里的 尝试传递一个捕获了外部变量的 Lambda 表达式时
	//（例如 lamda1 中的 [=] 表示捕获所有外部变量），这会创建一个闭包，其类型不再是一个简单的函数指针，而是一个复杂的类型。
	ForEach1(values, lamda1);

	//再用一个lamda函数实现找到数组中第一个大于3的数

	auto it = std::find_if(values.begin(), values.end(), [](int value) {return value > 3; });
	std::cout << *it << std::endl;
}