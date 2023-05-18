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
	reinterpret_cast������ǿ��ת���й�����Ϊǿ��ģ��������ײ㣬���ȫ�������ı����Ǳ�������ָ�
	�������ã������԰�һ��ָ��ת����һ��������Ҳ���԰�һ������ת����һ��ָ�롣���߲�ͬ���͵�ָ����໥�滻
	
	double a = 1.1;
	char* c = reinterpret_cast<char*>(&a);
	double* b = reinterpret_cast<double*>(c);
	cout << *b << endl;*/

	/* const_cast
	* ���������ܣ�ȥ��const�ͼ���const
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
	��������� expression ת��Ϊ type-id ���ͣ���û������ʱ���ͼ������֤ת���İ�ȫ�ԡ�����Ҫ�����¼����÷���

�� �������νṹ�л��������֮��ָ������õ�ת������������ת�����������ָ�������ת���ɻ����ʾ���ǰ�ȫ�ģ���������ת�����ѻ���ָ�������ת���������ʾ��ʱ������û�ж�̬���ͼ�飬�����ǲ���ȫ�ġ�
�� ���ڻ�����������֮���ת������� int ת���� char���� int ת���� enum������ת���İ�ȫ��ҲҪ������Ա����֤��
�� �ѿ�ָ��ת����Ŀ�����͵Ŀ�ָ�롣
�� ���κ����͵ı��ʽת����void��?*/
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
	dynamic_cast����̬ת����������������ʱ�̽�������ת�����Ӷ�ʹ�����ܹ���һ�����νṹ��ȫ��ת�����͡�
	dynamic_cast �ṩ������ת����ʽ���ѻ���ָ��ת����������ָ�룬���߰�ָ��������ֵת��������������á�
	*/
	return 0;
}