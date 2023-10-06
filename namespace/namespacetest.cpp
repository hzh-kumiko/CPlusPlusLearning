#include<iostream>
#include<string>
#include<algorithm>

namespace apple {
	namespace function {

		void print(const std::string& text) {
			std::cout << text << std::endl;
		}
	}
	void print(const std::string& text) {
		std::cout << text << std::endl;
	}
	
}

namespace orange {
	void print(const char* text) {
		std::string temp = text;
		std::reverse(temp.begin(), temp.end());
		std::cout << temp << std::endl;
	}
	void print_new() {

	}
}

int main() {
	//������namespace������namespace
	namespace a = apple::function;
	apple::print("mmmm");
	a::print("hello");
	orange::print("hello");

	//���ֱ�Ӳ���ǰ���orange::���֣�ֱ��using namespace orange;
	//���ֻ�ǲ�����ĳ������ǰ��:: �� using orange::print;
	//using namespace orange;
	using orange::print;
	print("myApple");
	orange::print_new();
}