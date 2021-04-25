#ifndef SNAKE_RG_FOOD_H_
#define SNAKE_RG_FOOD_H_

#include "Base.hpp"
#include <SFML/Graphics.hpp>

namespace rg {
	class Food : public BaseDrawable {
	public:
		Food(int outgame_size, int game_width, int game_height, int size);
		void generateNewPosition();
		int getX();
		int getY();
		void draw(sf::RenderWindow& window) override;
		void setColor(sf::Color new_color);
	private:
		int x, y;
		int outgame_size, game_width, game_height, size;
		sf::RectangleShape grap;
	};
}

#endif 
