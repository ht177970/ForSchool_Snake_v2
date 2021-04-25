#include "Food.hpp"
#include <random>

namespace rg {
	/*Food* Food::generateFood(int outgame_size, int game_width, int game_height, int size) {
		std::random_device random;
		std::mt19937 generator(random());
		int x = generator() % (game_width/size);//+1-1相抵
		int y = generator() % (game_height/size);//同上
		return new Food(outgame_size + x * size, outgame_size + y * size, size);
	}

	Food::Food(int X, int Y, int size) {
		x = X;
		y = Y;
		grap.setPosition(x, y);
		grap.setSize(sf::Vector2f(size, size));
		grap.setFillColor(sf::Color::Yellow);
	}*/

	Food::Food(int outgame_size, int game_width, int game_height, int size) {
		this->outgame_size = outgame_size;
		this->game_width = game_width;
		this->game_height = game_height;
		this->size = size;
		this->generateNewPosition();
	}

	void Food::generateNewPosition() {
		std::random_device random;
		std::mt19937 generator(random());
		this->x = outgame_size + (generator() % (game_width / size)) * size;//+1-1相抵
		this->y = outgame_size + (generator() % (game_height / size)) * size;//同上
		grap.setPosition(x, y);
		grap.setSize(sf::Vector2f(size, size));
		grap.setFillColor(sf::Color::Yellow);
	}

	int Food::getX() {
		return x;
	}

	int Food::getY() {
		return y;
	}

	void Food::draw(sf::RenderWindow& window) {
		window.draw(grap);
	}

	void Food::setColor(sf::Color new_color) {
		grap.setFillColor(new_color);
	}

	/*sf::RectangleShape Food::getDrawable() {
		return grap;
	}*/
}