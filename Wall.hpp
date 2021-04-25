#ifndef SNAKE_RG_WALL_H_
#define SNAKE_RG_WALL_H_

#include "Base.hpp"

namespace rg {
	class Wall : public BaseDrawable {
	public:
		explicit Wall(BaseData data);
		~Wall() = default;
		void draw(sf::RenderWindow& window) override;
		void setColor(sf::Color new_color);
	private:
		int m_outgame_size, m_ingame_width, m_ingame_height, m_snake_size;
		sf::Color m_color;
	};
}
#endif