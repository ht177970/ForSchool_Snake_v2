#include <iostream>
#include <fstream>
#include "Core.hpp"
#include <Windows.h>

sf::Texture image;
sf::Font font;

#define SUCCEED 100;

bool loadImage() {
	HRSRC rsrcData = FindResource(NULL, L"MainImage", RT_RCDATA);
	if (!rsrcData)
		throw std::runtime_error("Failed to find resource.");

	DWORD rsrcDataSize = SizeofResource(NULL, rsrcData);
	if (rsrcDataSize <= 0)
		throw std::runtime_error("Size of resource is 0.");

	HGLOBAL grsrcData = LoadResource(NULL, rsrcData);
	if (!grsrcData)
		throw std::runtime_error("Failed to load resource.");

	LPVOID firstByte = LockResource(grsrcData);
	if (!firstByte)
		throw std::runtime_error("Failed to lock resource.");

	if (!image.loadFromMemory(firstByte, rsrcDataSize))
		throw std::runtime_error("Failed to load image from memory.");
	return true;
}

int main() {
	
	if (!loadImage() || !font.loadFromFile("font.ttf")) {//!t.loadFromFile("snake.png")
		std::fstream f;
		f.open("error.log", std::ios_base::in | std::ios_base::out | std::ios_base::trunc);
		f << "載入遊戲必要檔案時發生錯誤(可能是檔案不存在)";
		f.close();
		throw std::runtime_error("Failed to load.");
	}

 	rg::Core c(font, image);
	c.Run();
	return 0;
}

