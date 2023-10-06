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
	//可以在namespace里再套namespace
	namespace a = apple::function;
	apple::print("mmmm");
	a::print("hello");
	orange::print("hello");

	//如果直接不在前面加orange::这种，直接using namespace orange;
	//如果只是不想在某个函数前加:: 就 using orange::print;
	//using namespace orange;
	using orange::print;
	print("myApple");
	orange::print_new();
}