#include "Menu.hpp"
#include "Core.hpp"

#define MENU_WITDH 600
#define MENU_HEIGHT 800

namespace rg {

#pragma region MainMenu

	MainMenu::MainMenu(sf::RenderWindow& window, renderManager& render) {
		this->window = &window;
		this->m_renderManager = &render;
		m_text_index = -1;
		this->window->setSize(sf::Vector2u(MENU_WITDH, MENU_HEIGHT));
		this->window->setView(sf::View(sf::FloatRect(0, 0, MENU_WITDH, MENU_HEIGHT)));
		this->window->setTitle("Snake MENU");
		this->m_renderManager->clearAllGraphics();
	}

	void MainMenu::initMenu() {
		sf::String texts[] = { L"開始遊戲", L"遊戲設定", L"離開" };
		int x = 300, y = 300, next_y = 120;
		sf::Font font;
		font.loadFromFile("font.ttf");

		m_renderManager->addGraphics(new Text(L"↑↓  選擇", font, -1, nullptr, 50, 760, 20U));
		m_renderManager->addGraphics(new Text(L"Enter確認", font, -1, nullptr, 50, 780, 20U));

		for (int i = 0; i < 3; i++)
			m_clickable_texts.push_back(new Text(texts[i], font, i, &m_text_index, x, y += next_y));
		for (Text* t : m_clickable_texts)
			m_renderManager->addGraphics(t);
		m_text_index = 0;

		m_renderManager->addGraphics(new Image(ImageBuilder("snake.png", 300, 200, 0.85f, 0.85f)));
		
		//display();
	}

	void MainMenu::display() {
		sf::Event e;
		/*sf::Clock clock;
		float time_step = 0.1f;*/
		//while (window->isOpen() && Core::getNowMode() == Mode::MAIN_MENU) {
			while(window->pollEvent(e))
				switch (e.type) {
				case sf::Event::Closed:
					window->close();
					return;
				case sf::Event::KeyPressed:
					switch (e.key.code) {
					case sf::Keyboard::Up:
						if (--m_text_index < 0)
							m_text_index = m_clickable_texts.size() - 1;
						break;
					case sf::Keyboard::Down:
						if (++m_text_index > m_clickable_texts.size() - 1)
							m_text_index = 0;
						break;
					case sf::Keyboard::Enter:
						EnterPressed();
						break;
					}
					break;
				}
			m_renderManager->Render();
		//}
	}

	void MainMenu::EnterPressed() {
		switch (m_text_index) {
		case 0:
			Core::changeState(Mode::GAMING);
			break;
		case 2:
			window->close();
			break;
		}
	}

#pragma endregion

	GameOverMenu::GameOverMenu(sf::RenderWindow& window, renderManager& render) {
		this->window = &window;
		this->m_renderManager = &render;
		this->m_renderManager->clearAllGraphics();
		m_text_index = -1;
	}

	void GameOverMenu::initMenu(int score, int highest_score) {
		std::wstringstream ss;
		ss << L" 你這次得到了 " << score << L" 分!";
		sf::Font font;
		font.loadFromFile("font.ttf");
		m_renderManager->addGraphics(new Text(ss.str(), font, -1, nullptr, 500, 200, 90U, sf::Color(0, 255, 0)));//color = lime
		ss.str(L"");
		ss << L"歷史最高 " << highest_score << L" 分";
		m_renderManager->addGraphics(new Text(ss.str(), font, -1, nullptr, 500, 290, 30U, sf::Color(143, 188, 143)));//color = orange

		sf::String texts[] = { L"回主選單", L"離開" };
		int x = 500, y = 280, next_y = 120;
		for (int i = 0; i < 2; i++)
			m_clickable_texts.push_back(new Text(texts[i], font, i, &m_text_index, x, y += next_y));
		for (Text* t : m_clickable_texts)
			m_renderManager->addGraphics(t);
		m_text_index = 0;

		//display();
	}

	void GameOverMenu::display() {
		sf::Event e;
		/*sf::Clock clock;
		float time_step = 0.1f;*/
		//while (window->isOpen() && Core::getNowMode() == Mode::GAMEOVER) {
			while (window->pollEvent(e))
				switch (e.type) {
				case sf::Event::Closed:
					window->close();
					return;
				case sf::Event::KeyPressed:
					switch (e.key.code) {
					case sf::Keyboard::Up:
						if (--m_text_index < 0)
							m_text_index = m_clickable_texts.size() - 1;
						break;
					case sf::Keyboard::Down:
						if (++m_text_index > m_clickable_texts.size() - 1)
							m_text_index = 0;
						break;
					case sf::Keyboard::Enter:
						EnterPressed();
						break;
					}
					break;
				}
			m_renderManager->Render();
		//}
	}

	void GameOverMenu::EnterPressed() {
		switch (m_text_index) {
		case 0:
			Core::changeState(Mode::MAIN_MENU);
			break;
		case 1:
			window->close();
			break;
		}
	}

#pragma region objects

	Text::Text(sf::String text, sf::Font font, int id, int* text_index, float x, float y, unsigned int size, sf::Color text_color) {
		this->m_id = id;
		this->m_font = font;
		this->grap = sf::Text(text, m_font, size);
		this->grap.setOrigin(grap.getLocalBounds().width / 2, grap.getLocalBounds().height / 2);
		this->grap.setFillColor(text_color);
		this->grap.setPosition(x, y);
		this->index = text_index;
	}

	int Text::getId() {
		return m_id;
	}

	void Text::draw(sf::RenderWindow& window) {
		updateText();
		window.draw(grap);
	}

	void Text::updateText() {
		if(this->index)
			grap.setFillColor((*this->index == m_id) ? sf::Color::Yellow : sf::Color::White);
	}

	Image::Image(ImageBuilder ib) {
		texture.loadFromFile(ib.p_location);
		sprite.setTexture(texture);
		sprite.setScale(sf::Vector2f(ib.zoom_x, ib.zoom_y));
		sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
		sprite.setPosition(ib.x, ib.y);
	}

	void Image::draw(sf::RenderWindow& window) {
		window.draw(sprite);
	}

#pragma endregion
}