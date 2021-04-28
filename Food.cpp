#include "Food.hpp"
#include <random>

namespace rg {

	Food::Food(BaseData data) {
		this->outgame_size = data.outgame_size;
		this->game_width = data.ingame_width;
		this->game_height = data.ingame_height;
		this->size = data.snake_size;
		this->generateNewPosition();
	}

	void Food::generateNewPosition() {
		std::random_device random;
		std::mt19937 generator(random());
		this->x = outgame_size + (generator() % (game_width / size)) * size;//+1-1¬Û©è
		this->y = outgame_size + (generator() % (game_height / size)) * size;//¦P¤W
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
}