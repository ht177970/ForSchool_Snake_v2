#include <iostream>
#include "Core.hpp"

sf::Texture t;
sf::Font f;

#define SUCCEED 100;

/*bool init();
int loadFile();*/

int main() {
	if (!t.loadFromFile("snake.png") || !f.loadFromFile("font.ttf"))
		return -1;
	
	rg::Core c(f, t);
	c.Run();
	return 0;
}
/*
bool init() {
	switch (loadFile()) {
	case IDCANCEL:
		return false;
	case IDTRYAGAIN:
		return init();
	}
	return true;
}

int loadFile() {
	if (!t.loadFromFile("snake.png") || !f.loadFromFile("font.ttf"))
		return MessageBox(NULL, L"缺少遊戲檔案\n可能導致無法正常遊戲\n如有疑問請聯繫作者(Ryan Tzeng)", L"遊戲檔案缺失", MB_ICONERROR | MB_CANCELTRYCONTINUE | MB_DEFBUTTON3);
	else
		return SUCCEED;
}*/

