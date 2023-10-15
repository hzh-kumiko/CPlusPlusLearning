# 1. 对于独占资源使用std::unique_ptr
## 1.1 基本用法
```
class Person {
	std::string Name;

public:

	template <typename T, typename = typename std::enable_if_t<
		!std::is_same<
			Person, std::decay_t<T>>::value>>
	explicit Person(T&& Rhs) {
		std::cout << "Universal Reference Constructor" << '\n';
	}

	Person(const Person& Rhs) {
		std::cout << "Copy Contructor" << '\n';
	}

	Person(Person&& Rhs) {
		std::cout << "Move Constructor" << '\n';
	}

	int Val = 10;
};

void Check(std::unique_ptr<Person>& u_ptr) {
	if (u_ptr.get()) {
		std::cout << "Has a pointer" << '\n';
	}
	else {
		std::cout << "Nullptr" << '\n';
	}
}

std::unique_ptr<Person> u_person_ptr1 = std::make_unique<Person>(10);       // make_unique 创建
std::unique_ptr<Person> u_person_ptr2(new Person(100));                     // new 对象创建
std::unique_ptr<Person> u_person_ptr3(new Person(1000));

std::cout << u_person_ptr1->Val << '\n';                                    // 10

Person* p_ptr1 = u_person_ptr1.get();                                       // 获取原始指针
Check(u_person_ptr1);                                                       // Has a pointer

Person* p_ptr2 = u_person_ptr2.release();                                   // 释放原始指针所有权，但不销毁原始指针
Check(u_person_ptr2);                                                       // Nullptr

u_person_ptr3.reset();                                                      // 重置，并且调用原始指针的析构函数
Check(u_person_ptr3);                                                       // Nullptr
```

## 1.2 默认情况下，std::unique_ptr的大小等同于原始指针，只可移动（不能拷贝）
### 默认情况下，std::unique_ptr的大小等同于原始指针，当析构时，std::unique_ptr销毁其指向的资源，默认情况下资源的析构通过调用原始指针中的delete实现
```
std::cout << sizeof(u_person_ptr1) << '\n';     // 8
```

## 1.3 std::unique_ptr可以使用自定义删除器
```
class A {

public:
	virtual ~A() {

	}
};

class B : public A {

};

class C : public A {

};

template <typename... T>
auto MakeA(T&&... Args) {
	auto DeleteFun = [](A* a) {
		std::cout << "Delete" << '\n';
		delete a;
	};
	std::unique_ptr<A, decltype(DeleteFun)> u_ptr(nullptr, DeleteFun);
	u_ptr.reset(new B());
	return u_ptr;
}

auto u_ptr = MakeA();
std::cout << sizeof(u_ptr) << '\n';        // 8
```

## 1.4 std::unique_ptr可以转换为std::shared_ptr
```
std::shared_ptr<int> s_ptr = std::make_unique<int>(1);
std::cout << *(s_ptr.get()) << '\n';                        // 1
```

#
# 2. std::shared_ptr
## 2.1 std::shared_ptr通过引用计数来判断其是否为指向某种资源的最后一个指针
### std::shared_ptr通过引用计数（reference count）来判断其是否为指向某种资源的某后一个指针。

### a. std::shared_ptr的大小是原始指针的两倍。其内部包含一个指向某种资源的原始指针，以及一个指向控制块（包含资源计数引用等）的原始指针。
### b. 对于控制块中计数引用的操作是原子性的。std::shared_ptr保证对于计数引用的操作是原子性的（线程安全），但是对于指向的资源不保证其操作的原子性
#
### std::shared_ptr构造函数递增计数引用值, 析构函数递减计数引用值, 拷贝赋值运算(递增计数引用值，之后递减计数引用值)。如果std::shared_ptr在计数值递减后发现引用计数值为零（即没有其他std::shared_ptr指向该资源）它就会调用原始指针的析构函数销毁资源。
### 从一个std::shared_ptr移动构造新std::shared_ptr会将原来的std::shared_ptr设置为null，那意味着老的std::shared_ptr不再指向资源，同时新的std::shared_ptr指向资源。这样的结果就是不需要修改引用计数值，因此移动std::shared_ptr会比拷贝它要快。

#
## 2.2 std::shared_ptr也支持自定义删除器
### std::shared_ptr支持自定义删除器，但是自定义删除器并不是std::shared_ptr的一部分（一般存放在std::shared_ptr指向的控制块中），因此自定义删除器并不会改变std::shared_ptr的大小
```
auto loggingDel = [](Widget *pw) {                    
                      makeLogEntry(pw);
                      delete pw; };

std::unique_ptr<Widget, decltype(loggingDel)> upw(new Widget, loggingDel);
std::shared_ptr<Widget> spw(new Widget, loggingDel); 
```

## 2.3 std::shared_ptr所指向的控制块
![Image text](Pics\shared_ptr_control_block.jpg)

## 2.4 控制块创建规则
### a. std::make_shared< T >(...)会创建一个新的控制块
### b. 通过原始指针创建std::shared_ptr会创建一个新的控制块
### c. 通过std::unique_ptr构造std::shared_ptr会创建一个新的控制块（std::unique_ptr会被置为nullptr）
```
std::shared_ptr<int> s_ptr1 = std::make_shared<int>(1);
std::cout << s_ptr1.use_count() << '\n';					// 1

int* i_ptr = new int(1);
std::shared_ptr<int> s_ptr2(i_ptr);
std::cout << s_ptr2.use_count() << '\n';					// 1

std::shared_ptr<int> s_ptr3 = std::make_unique<int>(1);
std::cout << s_ptr3.use_count() << '\n';					// 1
```

## 2.4 避免使用原始指针创建std::shared_ptr
### 避免使用原始指针创建std::shared_ptr, 由于通过原始指针创建std::shared_ptr会创建一个新的控制块（而不是增加计数引用），所以可能会导致原始指针被销毁多次，导致未定义的行为。
```
int* i_ptr = new int(1);
{	
	std::shared_ptr<int> s_ptr1(i_ptr);
	std::shared_ptr<int> s_ptr2(i_ptr);
}

// i_ptr被销毁两次，第二个销毁会产生未定义的行为
```

## 2.5 基本用法
```
std::shared_ptr<int> u_ptr1 = std::make_shared<int>(1);
std::shared_ptr<int> u_ptr2(u_ptr1);
std::shared_ptr<int> u_ptr3(u_ptr1);
std::cout << u_ptr1.use_count() << '\n';					// 3

auto i_ptr = u_ptr1.get();									// 获取原始指针

u_ptr2.reset();												// 重置，不再持有原始指针，原始资源对应计数引用减1
std::cout << u_ptr1.use_count() << '\n';					// 2
std::cout << u_ptr2.use_count() << '\n';					// 0

u_ptr3.reset(new int(10));									// 重置，持有新的原始指针，原始资源对应计数引用减1
std::cout << u_ptr1.use_count() << '\n';					// 1
std::cout << u_ptr3.use_count() << '\n';					// 1
```

#
# 3. std::weak_ptr
## 3.1 当std::shared_ptr有可能指向已经销毁的对象时，使用std::weak_ptr
### 当std::shared_ptr有可能指向已经销毁的对象时，使用std::weak_ptr。std::weak_ptr不会影响资源的计数引用，但是其可以检查所指向的资源是否悬空（dangle），通过检查对应控制块的计数引用是否为0（为0说明当前没有shared_ptr指向某个资源）
### 悬空（dangle）：指针指向的对象不存在
```
std::shared_ptr<int> s_ptr(new int(1));
std::weak_ptr<int> w_ptr(s_ptr);

std::cout << s_ptr.use_count() << '\n';		// 1

s_ptr.reset();								// 重置std::shared_ptr
std::cout << s_ptr.use_count() << '\n';		// 0
std::cout << w_ptr.expired() << '\n';		// 1, 检查std::weak_ptr指针是否悬空
```

## 3.2 访问std::weak_ptr所指向资源
### 访问std::weak_ptr所指向资源时，需要原子操作来检查std::weak_ptr指针是否悬空。但是std::weak_ptr不支持解引用，可以使用一下两种方法来获取原始指针
### a. 通过从std::weak_ptr创建std::shared_ptr
### b. 以std::weak_ptr来构造std::shared_ptr
### 当std::weak_ptr指针悬空时，创建的std::shared_ptr为空，计数引用为0
```
std::shared_ptr<int> s_ptr(new int(1));
std::weak_ptr<int> w_ptr(s_ptr);

{
	s_ptr.reset();

	std::shared_ptr<int> is_ptr = w_ptr.lock();		// convert to std::shared_ptr
	std::cout << is_ptr.use_count() << '\n';		// 0
}

{
	s_ptr.reset(new int(10));
	w_ptr = s_ptr;									// reset weak_ptr
	std::cout << w_ptr.expired() << '\n';			// 0
	std::shared_ptr<int> is_ptr = w_ptr.lock();	
	std::cout << is_ptr.use_count() << '\n';		// 2
}
```

## 3.3 std::weak_ptr使用场景
### a. 观察者模式
### Subject对控制Observers的生命周期不感兴趣，但是当Observers被销毁时，Subject不再对其发布新的消息，因此可以保存Observers的std::weak_pt来判断其是否已经销毁
![Image text](Pics\observer.jpg)
### b. 解决循环引用
```
class B;
class A {

public:
	~A() {
		std::cout << "A Delete" << '\n';
	}
	
	std::shared_ptr<B> s_b;
};

class B {

public:
	~B() {
		std::cout << "B Delete" << '\n';
	}

	std::shared_ptr<A> s_a;
};

std::shared_ptr<A> sa_ptr = std::make_shared<A>();
std::shared_ptr<B> sb_ptr = std::make_shared<B>();

sa_ptr->s_b = sb_ptr;
sb_ptr->s_a = sa_ptr;

std::cout << sa_ptr.use_count() << '\n';			// 2
std::cout << sb_ptr.use_count() << '\n';			// 2

class A, class B都存在shared_ptr保存对方的引用，因此存在循环引用而无法导致资源被销毁
```
![Image text](Pics\shared_ptr_circle_reference.jpg)
### 通过将一个成员变量改为weak_ptr来解决此问题
```
class B;
class A {

public:
	~A() {
		std::cout << "A Delete" << '\n';
	}
	
	std::weak_ptr<B> s_b;
};

class B {

public:
	~B() {
		std::cout << "B Delete" << '\n';
	}

	std::shared_ptr<A> s_a;
};

std::shared_ptr<A> sa_ptr = std::make_shared<A>();
std::shared_ptr<B> sb_ptr = std::make_shared<B>();

sa_ptr->s_b = sb_ptr;
sb_ptr->s_a = sa_ptr;

std::cout << sa_ptr.use_count() << '\n';	// 2
std::cout << sb_ptr.use_count() << '\n';	// 1
											// B delete, A delete
```

#
# 4. std::make_unique, std::make_shared
## 4.1 优先考虑使用std::make_unique, std::make_shared创建智能指针
### 优先考虑使用std::make_unique, std::make_shared创建智能指针，原因如下：
### a. 异常安全，使用new创建存在内存泄漏的风险
```
processWidget(std::shared_ptr<Widget>(new Widget), computePriority());	// May Cause Memory Leak

原因在于编译器将源代码转化为目标代码的方式

源代码顺序：
1. new Widget创建Widget对象
2. 创建std::shared_ptr对象
3. 调用computePriority()

编译器生成代码顺序（可能）：
1. new Widget创建Widget对象
2. 调用computePriority()
3. 创建std::shared_ptr对象

如果computePriority()出现异常，那么new Widget创建的Widget对象就会发生内存泄漏

processWidget(std::make_shared<Widget>(Args...), computePriority());	// No Memory Leak
```
### b. 效率提升，std::make_shared允许编译器生成更小，更快，更简洁的数据结构(原始资源和控制块放在一块new)
```
std::shared_ptr<Widget> spw(new Widget);

两次new内存分配：1. new Widget  2. new Control Block

auto spw = std::make_shared<Widget>();

一次new内存分配：std::make_shared分配一块内存，同时容纳原始资源和控制块
```

## 4.2 std::make_shared, std::make_unique的局限
### std::make_shared, std::make_unique存在局限
### a. 不支持自定义删除器
#
## 4.3 std::shared_ptr不建议使用make_shared的情形
### a. 存在自定义内存管理的类
### b. 原始资源较大，存在weak_ptr指向原始资源，且weak_ptr的生命周期比shared_ptr久
### 只要weak_ptr没有被销毁（引用某个控制块），那么控制块就不应该被销毁，使用make_shared将会导致原始资源和控制块一块被销毁（即使原始资源不再使用）
```
class ReallyBigType { … };

auto pBigObj =                          // 通过std::make_shared
    std::make_shared<ReallyBigType>();  // 创建一个大对象
                    
…           // 创建std::shared_ptrs和std::weak_ptrs
            // 指向这个对象，使用它们

…           // 最后一个std::shared_ptr在这销毁，
            // 但std::weak_ptrs还在

…           // 在这个阶段，原来分配给大对象的内存还分配着

…           // 最后一个std::weak_ptr在这里销毁；
            // 控制块和对象的内存被释放

class ReallyBigType { … };              // 和之前一样

std::shared_ptr<ReallyBigType> pBigObj(new ReallyBigType);
                                        // 通过new创建大对象

…           // 像之前一样，创建std::shared_ptrs和std::weak_ptrs
            // 指向这个对象，使用它们
            
…           // 最后一个std::shared_ptr在这销毁,
            // 但std::weak_ptrs还在；
            // 对象的内存被释放

…           // 在这阶段，只有控制块的内存仍然保持分配

…           // 最后一个std::weak_ptr在这里销毁；
            // 控制块内存被释放
```

#
# 5. Pimpl
## 5.1 Pimpl通过减少类的实现和类使用者之间的编译依赖来减少编译时间
### 将类中的数据成员替换为一个指向某种数据结构，struct的指针
```
// Widget.h

class Widget {

public:
	Widget() {}
	~Widget() {}

private:
	std::string Name;
	std::vector<double> Data;
	// Other Data
};

由于Widget中存在string，vector，自定义数据类型，因此Widget.h必须包含<string>, <vector>这些头文件。
这样会增加Widget使用者（include "Widget.h"）的编译时间，尤其是自定义数据结构会经常改变
```
### 可以将这些数据包含在一个struct中，Widget只持有一个指向该struct的指针
```

// Widget.h

class Widget {

public:
	Widget() {}
	~Widget() {}

private:
	struct Impl;
	std::unique_ptr<Impl> U_ptr;
};

// Widget.cpp

#include <string>
#include <vector>

#include "Widget.h"

struct Widget::Impl {
	std::string Name;
	std::vector<int> Data;
	// Other Data
};

Widget::Widget() : U_ptr(std::make_unique<Widget::Impl>()) {

}

// Main.cpp

#include "Widget.h"

int main() {
	Widget w;		// compile error, 使用未定义的类型Widget::Impl
	return 0;
}

原因：当编译器编译~Widget() {}时，会检查Widget::Impl是否为一个完整的类型。由于此时Widget::Impl只有声明，而没有定义，所以会报错。将~Widget()的定义放在Widget::Impl的声明之后即可

```

```
// Widget.h

#include <memory>

class Widget {

public:
	Widget();
	~Widget();		// only declare

private:
	struct Impl;
	std::unique_ptr<Impl> U_ptr;
};

// Widget.cpp

#include <string>
#include <vector>

#include "Widget.h"

struct Widget::Impl {
	std::string Name;
	std::vector<int> Data;
	// Other Data
};

Widget::Widget() : U_ptr(std::make_unique<Widget::Impl>()) {

}
Widget::~Widget() {}	// definition
```