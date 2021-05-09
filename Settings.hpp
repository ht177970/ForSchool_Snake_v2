#ifndef SNAKE_RG_SETTINGS_H_
#define SNAKE_RG_SETTINGS_H_

#include <SFML/Graphics.hpp>
#include <random>

namespace rg {
	class Settings {
	public:
		Settings() : main_pic() {}
		//Setter
		void setPic(sf::Texture pic) {
			main_pic = pic;
		}
		void setOutGameSize(int size) {
			outgame_size = size;
		}
		void setInGameWidth(int size) {
			ingame_width = size;
		}
		void setInGameHeight(int size) {
			ingame_height = size;
		}
		void setSnakeSize(int size) {
			snake_size = size;
		}
		void setFont(sf::Font font) {
			m_font = font;
		}
		void setColorMode(int index) {
			color_mode = index;
		}
		void setColorF(int index) {
			if (index == 0)
				color_f = 1000;
			else if (index == 1)
				color_f = 5;
			else
				color_f = 1;
		}
		//Getter
		int getOutGameSize() {
			return outgame_size;
		}
		int getInGameWidth() {
			return ingame_width;
		}
		int getInGameHeight() {
			return ingame_height;
		}
		int getSnakeSize() {
			return snake_size;
		}
		int getIngameScreenWidth() {
			return outgame_size * 2 + ingame_width;
		}
		int getIngameScreenHeight() {
			return outgame_size * 2 + ingame_height;
		}
		sf::Texture getPic() {
			return main_pic;
		}
		sf::Font getFont() {
			return m_font;
		}
		int getCCperFood() {
			return color_f;
		}
		int getColorF_index() {
			if (color_f == 1)
				return 2;
			else if (color_f == 5)
				return 1;
			else
				return 0;
		}
		int getColorMode() {
			return color_mode;
		}
		sf::Color getNewColor() {
			if (color_mode == 0)
				return sf::Color::Black;
			std::random_device random;
			std::mt19937 generator(random());
			switch (color_mode) {
			case 1:
				return sf::Color(generator() % 128 + 128, generator() % 128 + 128, generator() % 128 + 128);
			case 2:
				return sf::Color(generator() % 128, generator() % 128, generator() % 128);
			case 3:
				return sf::Color(generator() % 256, generator() % 256, generator() % 256);
			}
			return sf::Color::Black;
		}
	private:
		int outgame_size = 100;//witdh = height
		int ingame_width = 800, ingame_height = 600, snake_size = 20;
		int color_f = 1;//color change per food
		int color_mode = 3;//0 = always black(0), 1 = 淺色(0~127), 2 = 深色(128~255), 3 = 全彩(0~255)
		sf::Texture main_pic;
		sf::Font m_font;
	};
}

#endif