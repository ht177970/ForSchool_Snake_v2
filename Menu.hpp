#ifndef SNAKE_RG_MENU_H_
#define SNAKE_RG_MENU_H_

#include "WindowRender.hpp"
#include "Global.hpp"
#include <sstream>
#include <vector>
#include <SFML/System/String.hpp>

namespace rg {
	struct ImageBuilder {
		std::string p_location;
		float x;
		float y; 
		float zoom_x; 
		float zoom_y;
		ImageBuilder(std::string p_location, float x, float y, float zoom_x, float zoom_y) {
			this->p_location = p_location;
			this->x = x;
			this->y = y;
			this->zoom_x = zoom_x;
			this->zoom_y = zoom_y;
		}
	};

	class Text : public BaseDrawable {
	public:
		explicit Text(sf::String text, sf::Font font, int code, int* text_index, float x, float y, unsigned int size = 60U, sf::Color text_color = sf::Color::White);
		~Text() = default;
		int getId();
		void draw(sf::RenderWindow& window) override;
		void updateText();
	private:
		sf::Text grap;
		sf::Font m_font;
		int m_id;
		int* index;
	};

	class Image : public BaseDrawable {
	public:
		explicit Image(ImageBuilder ib);
		void draw(sf::RenderWindow& window) override;
	private:
		sf::Texture texture;
		sf::Sprite sprite;
	};

	class ClickableMenu {
	public:
		void display();
	protected:
		int m_text_index;
		sf::RenderWindow* window;
		renderManager* m_renderManager;
		std::vector<Text*> m_clickable_texts;
		virtual void EnterPressed() = 0;
	};

	class MainMenu : public ClickableMenu {
	public:
		explicit MainMenu(sf::RenderWindow& window, renderManager& render);
		~MainMenu() = default;
		void initMenu();
		void EnterPressed();
	};

	class GameOverMenu : public ClickableMenu {
	public:
		explicit GameOverMenu(sf::RenderWindow& window, renderManager& render);
		~GameOverMenu() = default;
		void initMenu(int score, int highest_score);
		void EnterPressed();
	};
}

#endif
