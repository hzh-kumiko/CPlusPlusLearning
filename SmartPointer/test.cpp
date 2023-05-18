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
	//1.δ��ʼ��,����
	/*
	int a = 1;
	int* p;
	cout << *p << endl;
	*/

	//2.����ָ��
	// ָ�뱻free��delete��,Ҫ��Ϊnull
	//dangling_point();
	
	//3.����ջ�ڴ�ָ�������
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
	//��Щ�ڴ���Դ�Ѿ����ͷţ���ָ������ָ�벢û�иı�ָ�򣨳�Ϊ��Ұָ�룩�����Һ�������ʹ�ã�
	//��Щ�ڴ���Դ�Ѿ����ͷţ���������ͼ���ͷ�һ�Σ��ظ��ͷ�ͬһ���ڴ�ᵼ�³������б�������
	//û�м�ʱ�ͷŲ���ʹ�õ��ڴ���Դ������ڴ�й©������ռ�õ��ڴ���ԴԽ��Խ�ࡣ
	//shared_ptr �Ǵ洢��̬���������ָ�룬����Ҫ�����ǹ���̬������������٣��Ӷ��������������ڴ�й©������ָ������⡣
	//ȱ�� ѭ������
	shared_ptr<int> sharedI(new int(100)); //���Ƽ���new
	
	shared_ptr<int> sharedII = make_shared<int>(200); //����
	//cout << *sharedI << *sharedII << endl;
	int* shared_p = new int(300);
	shared_ptr<int> sharedIII(shared_p);  //����������ָ���ʼ��
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
	
	//weak_ptr ���shared_ptrѭ�����õ�����
	weak_ptr<int> weakI(sharedI);
	cout << sharedI.unique() <<endl;

	//unique_ptr ��ռʽ����ָ��
	//��ֹ���ƹ��캯��
	unique_ptr<int> uniqueI = make_unique<int>(100);
	unique_ptr<int> uniqueII = make_unique<int>(200);
	//unique_ptr<int> uniqueII(uniqueI); //��ֹ���ƹ���
	//��ֹ��ֵ
	//uniqueII = uniqueI;
	//�����ƶ����죬
	unique_ptr<int> uniqueIII(move(uniqueI));
	//unique תshared,�����ٲ���ԭunique
	shared_ptr<int> shared_trans(move(uniqueII));
	cout << *shared_trans << endl;

	return 0;
}