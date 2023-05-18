#include<iostream>
#include"testA.h"
#include"testB.h"
#include<vld.h>
#include<memory>
using namespace std;
using namespace A;
class Test {
public:
	Test() {}
	Test(int i_):i(i_){}
	int i;
};
/*
void dangling_point() {
	char* p = (char*)malloc(10);
	strcpy(p, "abc");
	cout << p << endl;
	free(p);
	if (p != NULL) cout << p << endl;
	strcpy(p, "def");
}

*/
int* return_ret() {
	int result = 1;
	return &result;
}

int main() {
	//test();
	//B::test();

	//wild pointer
	//1.未初始化,报错
	/*
	int a = 1;
	int* p;
	cout << *p << endl;
	*/

	//2.悬垂指针
	// 指针被free或delete后,要设为null
	//dangling_point();
	
	//3.返回栈内存指针或引用
	/*
	int* p = return_ret();
	cout << *p << endl;
	cout << *p << endl;
	*/
	cout << sizeof(int) << " " << sizeof(int*) << endl;
	int* p = new int(100);
	string* s = new string;
	string* s1 = new string("aa");
	//cout << *p << endl;
	cout << *s << endl;
	cout << *s1 << endl;

	delete p;
	delete s;
	delete s1;

	int* num = new int[100]();
	num[15] = 1;
	cout << num[10] <<num[15]<< endl;
	delete[] num;
	
	Test *t = new Test[10];
	t[1] =  Test(5);
	cout << t[1].i << endl;
	delete[] t;

	//shared_ptr
	//有些内存资源已经被释放，但指向它的指针并没有改变指向（成为了野指针），并且后续还在使用；
	//有些内存资源已经被释放，后期又试图再释放一次（重复释放同一块内存会导致程序运行崩溃）；
	//没有及时释放不再使用的内存资源，造成内存泄漏，程序占用的内存资源越来越多。
	//shared_ptr 是存储动态创建对象的指针，其主要功能是管理动态创建对象的销毁，从而帮助彻底消除内存泄漏和悬空指针的问题。
	//缺陷 循环引用
	shared_ptr<int> sharedI(new int(100)); //不推荐用new
	
	shared_ptr<int> sharedII = make_shared<int>(200); //常用
	//cout << *sharedI << *sharedII << endl;
	int* shared_p = new int(300);
	shared_ptr<int> sharedIII(shared_p);  //不适宜用裸指针初始化
	//cout << *sharedIII << endl;
	cout << "size " << sizeof(sharedIII) <<" " <<sizeof(shared_p)<<endl;
	shared_ptr<int> sharedIV(sharedII);
	//cout << sharedII.unique() << endl;
	//sharedIV.reset(new int(400));
	//cout << sharedII.unique() << endl;
	//delete shared_p;
	//cout << sharedI<<" "<< sharedII << " " << sharedIV << endl;
	swap(sharedII, sharedI);
	//cout << sharedII <<" "<< sharedI << endl;
	sharedI.swap(sharedII);
	//cout << sharedII << " "<< sharedI << endl;

	//shared_ptr<int> shared_nums(new int[100]());
	//shared_nums.get()[5] = 5;
	//cout << shared_nums.get()[5] << endl;
	cout << sizeof(sharedI) << endl;
	
	//weak_ptr 解决shared_ptr循环引用的问题
	weak_ptr<int> weakI(sharedI);
	cout << sharedI.unique() <<endl;

	//unique_ptr 独占式智能指针
	//禁止复制构造函数
	unique_ptr<int> uniqueI = make_unique<int>(100);
	unique_ptr<int> uniqueII = make_unique<int>(200);
	//unique_ptr<int> uniqueII(uniqueI); //禁止复制构造
	//禁止赋值
	//uniqueII = uniqueI;
	//允许移动构造，
	unique_ptr<int> uniqueIII(move(uniqueI));
	//unique 转shared,不能再操作原unique
	shared_ptr<int> shared_trans(move(uniqueII));
	cout << *shared_trans << endl;

	return 0;
}