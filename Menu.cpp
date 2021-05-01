#include "Menu.hpp"
#include "Core.hpp"

#define MENU_WITDH 600
#define MENU_HEIGHT 800

namespace rg {

	void ClickableMenu::display() {
		sf::Event e;
		while (window->pollEvent(e))
			switch (e.type) {
			case sf::Event::Closed:
				window->close();
				return;
			case sf::Event::KeyPressed:
				onKeyDown(e.key.code);
				break;
			case sf::Event::MouseMoved:
				onMouseMove(e.mouseMove);
				break;
			case sf::Event::MouseButtonPressed:
				onMouseClick();
				break;
			}
		m_renderManager->Render();
	}

	int ClickableMenu::changeTextIndex(int new_index) {
		this->m_text_index = new_index;
		return this->m_text_index;
	}

	void ClickableMenu::onKeyDown(sf::Keyboard::Key keycode) {
		switch (keycode) {
		case sf::Keyboard::Up:
			if (changeTextIndex(m_text_index - 1) < 0)
				changeTextIndex(m_clickable_texts.size() - 1);
			break;
		case sf::Keyboard::Down:
			if (changeTextIndex(m_text_index + 1) > m_clickable_texts.size() - 1)
				changeTextIndex(0);
			break;
		case sf::Keyboard::Enter:
			EnterPressed(m_text_index);
			break;
		}
	}

	void ClickableMenu::onMouseMove(sf::Event::MouseMoveEvent mouse) {
		for (auto t : m_clickable_texts)
			if (t->isPosIn(mouse.x, mouse.y)) {
				changeTextIndex(t->getId());
				break;
			}
	}

	void ClickableMenu::onMouseClick() {
		sf::Vector2i mouse = sf::Mouse::getPosition(*window);
		for (auto t : m_clickable_texts)
			if (t->isPosIn(mouse.x, mouse.y)) {
				EnterPressed(t->getId());
				break;
			}
	}

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
		m_renderManager->addGraphics(new MainImage());

		sf::String texts[] = { L"開始遊戲", L"遊戲設定", L"離開" };
		int x = 300, y = 300, next_y = 120;

		Text* help1 = new Text(L"↑↓  選擇", 49, 766);
		Text* help2 = new Text(L"Enter確認", 49, 786);
		help1->setTextSize(20U);
		help2->setTextSize(20U);
		m_renderManager->addGraphics(help1);
		m_renderManager->addGraphics(help2);

		for (int i = 0; i < 3; i++) {
			auto tmp = new Text(texts[i], x, y += next_y);
			tmp->setId(i);
			tmp->setTextIndexPointer(&m_text_index);
			m_clickable_texts.push_back(tmp);
		}
		for (Text* t : m_clickable_texts)
			m_renderManager->addGraphics(t);
		m_text_index = 0;
	}

	void MainMenu::EnterPressed(int index) {
		switch (index) {
		case 0:
			Global::C_changeCMode(CMode::GAMING);
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

		int t_x = Global::settings.getIngameScreenWidth() / 2, t_y = Global::settings.getIngameScreenHeight() / 4;
		unsigned int text_size;
		text_size = static_cast<unsigned int>(90.f * t_x / 500);

		Text* now_score_t = new Text(ss.str(), t_x, t_y);//x = 500(1000), y = 200(800)
		now_score_t->setTextSize(text_size);//90(1000)
		now_score_t->setTextColor(sf::Color(0, 255, 0));
		m_renderManager->addGraphics(now_score_t);//color = lime

		ss.str(L"");
		ss << L"歷史最高 " << highest_score << L" 分";
		Text* highest_score_t = new Text(ss.str(), t_x, t_y + text_size);
		text_size = static_cast<unsigned int>(30.f * t_x / 500);
		highest_score_t->setTextSize(text_size);
		highest_score_t->setTextColor(sf::Color(143, 188, 143));
		m_renderManager->addGraphics(highest_score_t);//color = orange

		sf::String texts[] = { L"再玩一次", L"回主選單", L"離開" };
		t_y += text_size * 4;
		text_size = static_cast<unsigned int>(60.f * t_x / 500);
		for (int i = 0; i < 3; i++) {
			auto tmp = new Text(texts[i], t_x, t_y += text_size*2);
			tmp->setTextSize(text_size);
			tmp->setId(i);
			tmp->setTextIndexPointer(&m_text_index);
			m_clickable_texts.push_back(tmp);
		}
		for (Text* t : m_clickable_texts)
			m_renderManager->addGraphics(t);
		m_text_index = 0;
	}

	void GameOverMenu::EnterPressed(int index) {
		switch (index) {
		case 0:
			Global::C_changeCMode(CMode::GAMING);
			break;
		case 1:
			Global::C_changeCMode(CMode::MAIN_MENU);
			break;
		case 2:
			window->close();
			break;
		}
	}

#pragma region objects

	Text::Text(sf::String text, float x, float y) {
		this->m_id = -1;//default
		this->m_font = Global::settings.getFont();
		this->grap = sf::Text(text, m_font, 60U);//default
		this->grap.setOrigin(grap.getLocalBounds().width / 2, grap.getLocalBounds().height / 2);
		this->grap.setPosition(x, y);
		this->index = nullptr;
	}

	void Text::setId(int id) {
		this->m_id = id;
	}

	void Text::setTextIndexPointer(int* text_index) {
		this->index = text_index;
	}

	void Text::setTextSize(unsigned int size) {
		this->grap.setCharacterSize(size);
		this->grap.setOrigin(grap.getLocalBounds().width / 2, grap.getLocalBounds().height / 2);
	}

	void Text::setTextColor(sf::Color color) {
		this->grap.setFillColor(color);
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

	bool Text::isPosIn(int x, int y) {
		sf::FloatRect grap_rect = grap.getLocalBounds();
		int left = grap.getPosition().x - grap_rect.width / 2 + grap_rect.left, right = left + grap_rect.width;
		int top = grap.getPosition().y - grap_rect.height / 2 + grap_rect.top, bottom = top + grap_rect.height;
		return top <= y && y <= bottom && left <= x && x <= right;
	}

	MainImage::MainImage() {
		texture = Global::settings.getPic();
		sprite.setTexture(texture);
		sprite.setPosition(0, 0);
	}

	void MainImage::draw(sf::RenderWindow& window) {
		window.draw(sprite);
	}

#pragma endregion
}