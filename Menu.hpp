#ifndef SNAKE_RG_MENU_H_
#define SNAKE_RG_MENU_H_

#include "WindowRender.hpp"
#include "Global.hpp"
#include <sstream>
#include <vector>
#include <SFML/System/String.hpp>

namespace rg {

	class Text : public BaseDrawable {
	public:
		explicit Text(sf::String text, float x, float y);
		~Text() = default;
		void setId(int id);
		void setTextIndexPointer(int* text_index);
		void setTextSize(unsigned int size);
		void setTextColor(sf::Color color);
		int getId();
		void draw(sf::RenderWindow& window) override;
		bool isPosIn(int x, int y);
	private:
		sf::Text grap;
		sf::Font m_font;
		int m_id;
		int* index;
		void updateText();
	};

	class MainImage : public BaseDrawable {
	public:
		explicit MainImage();
		void draw(sf::RenderWindow& window) override;
	private:
		sf::Texture texture;
		sf::Sprite sprite;
	};

	class BaseMenu {
	public:
		virtual void display() = 0;
	protected:
		sf::RenderWindow* window;
		renderManager* m_renderManager;
	};

	class ClickableMenu : protected BaseMenu {
	public:
		void display();
	protected:
		int m_text_index;
		std::vector<Text*> m_clickable_texts;
		int changeTextIndex(int new_index);
		void onKeyDown(sf::Keyboard::Key keycode);
		void onMouseMove(sf::Event::MouseMoveEvent mouse);
		void onMouseClick();
		virtual void EnterPressed(int index) = 0;
	};

	class MainMenu : public ClickableMenu {
	public:
		explicit MainMenu(sf::RenderWindow& window, renderManager& render);
		~MainMenu() = default;
		void initMenu();
		void EnterPressed(int index);
	};

	class GameOverMenu : public ClickableMenu {
	public:
		explicit GameOverMenu(sf::RenderWindow& window, renderManager& render);
		~GameOverMenu() = default;
		void initMenu(int score, int highest_score);
		void EnterPressed(int index);
	};
}

#endif
