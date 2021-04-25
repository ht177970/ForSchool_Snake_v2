#ifndef SNAKE_RG_BASE_H_
#define SNAKE_RG_BASE_H_

#include <SFML/Graphics.hpp>

namespace rg {
	class BaseDrawable {
	public:
		virtual ~BaseDrawable() = default;
		virtual void draw(sf::RenderWindow& window) = 0;
	};

	struct Pos {
		int x, y;
		bool operator==(Pos other) {
			return (x == other.x && y == other.y);
		}
	};

	struct BaseData {
		int outgame_size, ingame_width, ingame_height, snake_size;
		BaseData(int outgame_size, int ingame_width, int ingame_height, int snake_size) {
			this->outgame_size = outgame_size;
			this->ingame_width = ingame_width;
			this->ingame_height = ingame_height;
			this->snake_size = snake_size;
		}
	};
}

#endif