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
	protected:
		sf::Text grap;
		sf::Font m_font;
		int m_id;
		int* index;
		virtual void updateText();
	};

	class SettingsText : public Text {
	public:
		explicit SettingsText(std::vector<sf::String> texts, float x, float y, int default_index);
		void onKeyDown(sf::Keyboard::Key keycode);
		int getSubIndex() { return index; };
	protected:
		void updateText() override;
	private:
		int x, y;
		int index;
		bool tri_visible_left, tri_visible_right;
		sf::VertexArray tri_left, tri_right;
		std::vector<sf::String> texts;
		void updateTri();
		void draw(sf::RenderWindow& window) override;
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
		virtual int getClickableSize() { return m_clickable_texts.size(); };
		virtual std::vector<Text*> getBaseClickable() { return m_clickable_texts; };
	protected:
		ClickableMenu(sf::RenderWindow& window, renderManager& render);
		int m_text_index;
		std::vector<Text*> m_clickable_texts;
		int changeTextIndex(int new_index);
		void onKeyDown(sf::Keyboard::Key keycode);
		void onMouseMove(sf::Event::MouseMoveEvent mouse);
		void onMouseClick();
		virtual void otherKeyDown(sf::Keyboard::Key keycode) {};
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

	class SettingsMenu : public ClickableMenu {
	public:
		explicit SettingsMenu(sf::RenderWindow& window, renderManager& render);
		~SettingsMenu() = default;
		void initMenu();
		void EnterPressed(int index);
		void otherKeyDown(sf::Keyboard::Key keycode) override;
		int getClickableSize() override { return s_clickable_texts.size(); };
		std::vector<Text*> getBaseClickable() override;
	private:
		bool setting;
		std::vector<SettingsText*> s_clickable_texts;
	};
}

#endif
