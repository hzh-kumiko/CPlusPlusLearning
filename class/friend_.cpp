#include<iostream>
#include<ostream>
#include<vector>
void print() {
	std::cout << "The friend function of Screen." << std::endl;
}
class Screen {
	friend std::ostream& operator<<(std::ostream& os, const Screen& s) {
		os << s.height << " " << s.width << std::endl;
		os << s.contents << std::endl;
		return os;
	}
	friend void print();
	friend class Window_mgr;
public:
	typedef std::string::size_type pos;
	Screen() = default;
	Screen(pos ht, pos wd, char c) :height(ht), width(wd), contents(ht* wd, c) {}
	char get() const {
		print();
		return contents[cursor];
	}
	char get(pos ht, pos wd) {
		pos row = ht * width;
		
		return contents[row + wd];
	}
private:
	pos cursor = 0;
	pos height = 0, width = 0;
	std::string contents;
};

class Window_mgr {
public:
	using ScreenIndex = std::vector<Screen>::size_type;
	void clear(ScreenIndex);
	Screen& get(ScreenIndex i) {
		return screens[i];
	}
	ScreenIndex addScreen(const Screen& s);
private:
	std::vector<Screen> screens{ Screen(24,80,'1') };
};

void Window_mgr::clear(ScreenIndex i) {
	Screen& s = screens[i];
	s.contents = std::string(s.height * s.width, ' ');
}
inline Window_mgr::ScreenIndex Window_mgr::addScreen(const Screen& s) {
	screens.push_back(s);
	return screens.size() - 1;
}

//Window_mgr作为Screen的友元类，可以访问Screen的私有成员
int main() {
	Screen s(3, 4, 'd');
	s.get();
	std::cout << s << std::endl;
	Window_mgr wm;

	//verify the function of clear()
	std::cout << wm.get(0) << std::endl;
	wm.clear(0);
	std::cout << wm.get(0) << std::endl;

	//verify the function of add
	std::cout << wm.get(wm.addScreen(Screen(1, 3, 'm'))) << std::endl;
	  
}