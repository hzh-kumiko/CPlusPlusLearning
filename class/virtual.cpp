#include<iostream>

using namespace std;

struct test {
public:
	test(int i, int j,int k):i1(i),i2(j),p(new int(k)){}  //��ͨ���캯��
	test(const test& t1) {    //�������캯��
		i1 = t1.i1;
		i2 = t1.i2;
		p = new int(*t1.p);
	}
	~test() {
		delete p;
		cout << "test ended" << endl;
	}
	int i1;
	int i2;
	int* p;
};

class thisTest {
public:
	thisTest(string name_, int num_):name(name_),num(num_){}
	~thisTest(){}
	void print() const {
		cout << name << " " << num << endl;
	}
private:
	string name;
	int num;
};

//�̳�
// ����
class A {
public:
	virtual void fun(int a) { // �麯��
		cout << "This is A fun " << a << endl;
	}
	void add(int a, int b) {
		cout << "This is A add " << a + b << endl;
	}
	void print() {
		cout << "A print" << endl;
	}
};

// ����
class B : public A {
public:
	void fun(int a) override {  // ����
		cout << "this is B fun " << a << endl;
	}
	void add(int a) {   // ����
		cout << "This is B add " << a + a << endl;
	}
	virtual void print() {
		cout << "B print" << endl;
	}
	void inlinetest();
};

int main() {

	// ����ָ��ָ�����������ʱ������ָ�����ֱ�ӵ��õ�������ĸ��Ǻ�����Ҳ����ͨ�� :: ���õ����౻����
	// ���麯����������ָ��ֻ�ܵ��û���ı����غ������޷�ʶ���������е����غ�����
	// �ṹ���ڲ��ĳ�Ա������inline�ģ��ⲿҲ����������inline��

	A* p = new B();
	p->fun(1);      // �������� fun ���Ǻ���
	p->A::fun(1);   // ���ø��� fun
	p->add(1, 2);
	// p->add(1);      // ����ʶ����� A ���е� add ������������ƥ��
	// p->B::add(1);   // �����޷�ʶ������ add ����
	p->print();
	return 0;
}
