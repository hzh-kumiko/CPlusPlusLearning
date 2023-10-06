#include <iostream>
#include <vector>
#include<functional>

void print(int a) {
	std::cout << "value: " << a << std::endl;
}
//����ָ��������
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

typedef void(*printFunction)(int);  //typedef����ָ��
typedef long long ll;     //��˵�����滻

int main()
{
	ll a;
	a = 1e11;
	std::cout << a << std::endl;
	auto function = &print;
	auto function1 = print;  //���൱����һ�У�������ʽת�� �����ֵ�print�����ص�ʱ������Ҫ�ó���ģ�Ҳ���������һ����ʽ�����庯��ָ��
	void(*test)(int);
	test = print;  //����һ������ָ�룬ָ�����print
	std::cout << function << std::endl;
	std::cout << function1 << std::endl;
	function(1);
	printFunction myFunction = print;  //��3����һ����
	test(4);
	myFunction(5);

	std::vector<int> values = { 1,2,3,5,4};
	ForEach(values, print); //myFunction,print,test ������

	std::cout << std::endl;
	//lamda���ʽ������ָ��
	auto lamda = [](int value) {std::cout << value << std::endl; };
	ForEach(values, lamda);
	std::cout << std::endl;
	//��������Ҫ����һ���ⲿ�����ݣ���Ҫ��[]�����= �� & ����ʾֵ���������ⲿ����
	// �����ô��ݣ� ֵ�����ǿ����� ���ò���
	//ͬʱ = �� & ����ͬʱ����ͬһ������ ���� [=a,&a]������
	int a_pass = 1;

	auto lamda1 = [=](int value) {std::cout << value << std::endl; };
	//����lamda1�Ͳ�����ForEach����ΪForEach��� ���Դ���һ���������ⲿ������ Lambda ���ʽʱ
	//������ lamda1 �е� [=] ��ʾ���������ⲿ����������ᴴ��һ���հ��������Ͳ�����һ���򵥵ĺ���ָ�룬����һ�����ӵ����͡�
	ForEach1(values, lamda1);

	//����һ��lamda����ʵ���ҵ������е�һ������3����

	auto it = std::find_if(values.begin(), values.end(), [](int value) {return value > 3; });
	std::cout << *it << std::endl;
}