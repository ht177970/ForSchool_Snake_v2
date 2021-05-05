#include <iostream>
#include <fstream>
#include "Core.hpp"
#include <fstream>

sf::Texture t;
sf::Font f;

#define SUCCEED 100;

/*bool init();
int loadFile();*/

int main() {
	if (!t.loadFromFile("snake.png") || !f.loadFromFile("font.ttf")) {
		std::fstream f;
		f.open("error.log", std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
		f << "載入遊戲必要檔案時發生錯誤(可能是檔案不存在)";
		f.close();
		return -1;
	}

	/*std::fstream fs;
	fs.open("temp");
	long l;
	fs >> l;
	if ((l - 17) % 131)
		l = 0;
	else
		l = (l - 17) / 131;*/
	
 	rg::Core c(f, t);
	c.Run();
	return 0;
}
//MessageBox與SFML衝突
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

