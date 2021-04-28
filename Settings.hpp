#ifndef SNAKE_RG_SETTINGS_H_
#define SNAKE_RG_SETTINGS_H_

#include <SFML/Graphics.hpp>

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
	private:
		int outgame_size = 100;//witdh = height
		int ingame_width = 800, ingame_height = 600, snake_size = 20;
		sf::Texture main_pic;
		sf::Font m_font;
	};
}

#endif