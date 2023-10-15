#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
using namespace std;

class A {
public:
	int num1, num2;
	explicit A() :num1(0), num2(0) { cout << "simple" << endl; }
	A(int n1, int n2) : num1(n1), num2(n2) { cout << "ֵ���ݹ���" << endl; }
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
		cout << "=����������������" << endl;
		num1 = a.num1 + 1;
		num2 = a.num2 + 1;
		return *this;
	}
	A& operator=(vector<int>& nums) {
		cout << "=��������ֵ����" << endl;
		num1 = nums[0];
		num2 = nums[1];
		return *this;
	}
};

int main() {

	A a1;
	A a2(1, 3); //���캯��
	A a3 = { 2, 3 }; //���캯�� ��������һ��,���ǿ������� ����ʼ�����캯������Ϊ��ʽ��ʱ�����ֱ��ʹ��A a3 = {2,3}�Ͳ�����
	A a3_(A( 2, 4 )); // ֱ�ӽ���ʱ���� A(2, 4) ������� a3_����û�ж���Ŀ��������� a2,a3,a3_���ǳ�ʼ������
	cout << a3_.num2 << endl;
	cout << "----------" << endl;
	//��������
	A a_4(a2); //��������
	A a4 = a2; //�������캯�� �������캯������Ϊ��ʽ��ʱ��Ͳ�������ֻ��A a4(a2);
	cout << "--------" << endl;
	A a5; //Ĭ�Ϲ��캯��
	a5 = a2; //=���ع���
	
	a5 = { 1,7 }; //=���ع��� ����{1,7} ���ù��캯�����ٿ�������
	A a6;
	vector<int> n = { 1,7 };
	a6 = n;
	A a7(move(a2));
}