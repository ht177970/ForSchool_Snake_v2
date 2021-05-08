#include "Menu.hpp"
#include "Core.hpp"

#define MENU_WITDH 600
#define MENU_HEIGHT 800

namespace rg {

#pragma region ClickableMenu

	ClickableMenu::ClickableMenu(sf::RenderWindow& window, renderManager& render) {
		this->window = &window;
		this->m_renderManager = &render;
		m_text_index = -1;
		this->m_renderManager->clearAllGraphics();
	}

	void ClickableMenu::display() {
		sf::Event e;
		while (window->pollEvent(e))
			switch (e.type) {
			case sf::Event::Closed:
				window->close();
				return;
			case sf::Event::KeyReleased:
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
				changeTextIndex(this->getClickableSize() - 1);
			break;
		case sf::Keyboard::Down:
			if (changeTextIndex(m_text_index + 1) > this->getClickableSize() - 1)
				changeTextIndex(0);
			break;
		case sf::Keyboard::Enter:
			EnterPressed(m_text_index);
			break;
		default:
			otherKeyDown(keycode);
			break;
		}
	}

	void ClickableMenu::onMouseMove(sf::Event::MouseMoveEvent mouse) {
		for (auto t : this->getBaseClickable())
			if (t->isPosIn(mouse.x, mouse.y)) {
				changeTextIndex(t->getId());
				break;
			}
	}

	void ClickableMenu::onMouseClick() {
		sf::Vector2i mouse = sf::Mouse::getPosition(*window);
		for (auto t : this->getBaseClickable())
			if (t->isPosIn(mouse.x, mouse.y)) {
				EnterPressed(t->getId());
				break;
			}
	}

#pragma endregion

#pragma region MainMenu

	MainMenu::MainMenu(sf::RenderWindow& window, renderManager& render) : ClickableMenu(window, render) {
		this->window->setSize(sf::Vector2u(MENU_WITDH, MENU_HEIGHT));
		this->window->setView(sf::View(sf::FloatRect(0, 0, MENU_WITDH, MENU_HEIGHT)));
		this->window->setTitle("Snake MENU");
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
		case 1:
			Global::C_changeCMode(CMode::SETTINGS);
			break;
		case 2:
			window->close();
			break;
		}
	}

#pragma endregion

#pragma region GameOverMenu

	GameOverMenu::GameOverMenu(sf::RenderWindow& window, renderManager& render) : ClickableMenu(window, render) { }

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

#pragma endregion

#pragma region SettingsMenu

	SettingsMenu::SettingsMenu(sf::RenderWindow& window, renderManager& render) : ClickableMenu(window, render) {
		setting = false;
	}

	void SettingsMenu::initMenu() {
		m_renderManager->addGraphics(new MainImage());

		sf::String texts[] = { L"遊戲變色模式", L"遊戲變色頻率"};
		std::vector<sf::String> subs[] = { 
			{L"黑色", L"淺色", L"深色", L"彩色"},
			{L"關", L"慢", L"快"}
		};
		int default_index[] = { Global::settings.getColorMode(), Global::settings.getColorF_index()};
		int x = 200, y = 200, next_y = 120;

		Text* help1 = new Text(L"↑↓  選擇", 49, 766);
		Text* help2 = new Text(L"Enter確認", 49, 786);
		help1->setTextSize(20U);
		help2->setTextSize(20U);
		m_renderManager->addGraphics(help1);
		m_renderManager->addGraphics(help2);

		int id = 0;

		SettingsText* saveSettings = new SettingsText({ L"儲存並返回" }, 300, 600, 0);
		saveSettings->setTextIndexPointer(&m_text_index);
		saveSettings->setId(id++);
		s_clickable_texts.push_back(saveSettings);

		SettingsText* n_saveSettings = new SettingsText({ L"不儲存返回" }, 300, 700, 0);
		n_saveSettings->setTextIndexPointer(&m_text_index);
		n_saveSettings->setId(id++);
		s_clickable_texts.push_back(n_saveSettings);

		for (int i = 0; i < 2; i++) {
			auto settings_name = new Text(texts[i], x, y += next_y);
			m_renderManager->addGraphics(settings_name);

			auto settings_choose = new SettingsText(subs[i], x + 300, y, default_index[i]);
			settings_choose->setId(i + id);
			settings_choose->setTextIndexPointer(&m_text_index);
			settings_choose->setTextSize(40U);
			s_clickable_texts.push_back(settings_choose);
		}

		for (Text* t : s_clickable_texts)
			m_renderManager->addGraphics(t);

		m_text_index = 2;
	}

	std::vector<Text*> SettingsMenu::getBaseClickable() {
		std::vector<Text*> tmp;
		for (auto clickable : s_clickable_texts)
			tmp.push_back(clickable);
		return tmp;
	}

	void SettingsMenu::EnterPressed(int index) { 
		if (index == 0) {//0 = save
			Global::settings.setColorMode(s_clickable_texts[2]->getSubIndex());//2 = color settings
			Global::settings.setColorF(s_clickable_texts[3]->getSubIndex());//3 = color change per second
			Global::C_changeCMode(CMode::MAIN_MENU);
		}
		else if (index == 1) {//1 = no save
			Global::C_changeCMode(CMode::MAIN_MENU);
		}
	}

	void SettingsMenu::otherKeyDown(sf::Keyboard::Key keycode) {
		if (keycode == sf::Keyboard::Left || keycode == sf::Keyboard::Right)
			s_clickable_texts[m_text_index]->onKeyDown(keycode);
	}


#pragma endregion


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
			this->setTextColor((*this->index == m_id) ? sf::Color::Yellow : sf::Color::White);
	}

	bool Text::isPosIn(int x, int y) {
		sf::FloatRect grap_rect = grap.getLocalBounds();
		//int left = grap.getPosition().x - grap_rect.width / 2 + grap_rect.left, right = left + grap_rect.width;
		//int top = grap.getPosition().y - grap_rect.height / 2 + grap_rect.top, bottom = top + grap_rect.height;
		/*return grap.getPosition().x - grap_rect.width / 2 + grap_rect.left <= x && x <= grap.getPosition().x + grap_rect.width / 2 + grap_rect.left &&
			grap.getPosition().y - grap_rect.height / 2 + grap_rect.top <= y && y <= grap.getPosition().y + grap_rect.height / 2 + grap_rect.top;*/
		x -= grap.getPosition().x + grap_rect.left - grap_rect.width / 2;
		y -= grap.getPosition().y + grap_rect.top - grap_rect.height / 2;
		return 0 <= x && x <= grap_rect.width &&
			0 <= y && y <= grap_rect.height;
	}

	MainImage::MainImage() {
		texture = Global::settings.getPic();
		sprite.setTexture(texture);
		sprite.setPosition(0, 0);
	}

	void MainImage::draw(sf::RenderWindow& window) {
		window.draw(sprite);
	}

	SettingsText::SettingsText(std::vector<sf::String> texts, float x, float y, int default_index) : Text(texts[default_index], x, y),
		tri_left(sf::Triangles, 3), tri_right(sf::Triangles, 3) {

		this->texts = texts;
		this->x = x;
		this->y = y;
		index = default_index;
		tri_visible_left = false;
		tri_visible_right = false;
		//updateTri();
	}

	void SettingsText::draw(sf::RenderWindow& window) {
		Text::draw(window);
		if (*Text::index == this->m_id && texts.size() > 1) {
			window.draw(tri_left);
			window.draw(tri_right);
		}
	}

	void SettingsText::updateText() {
		grap.setString(texts[index]);
		grap.setOrigin(grap.getLocalBounds().width / 2, grap.getLocalBounds().height / 2);
		if (*Text::index == this->m_id) {
			this->setTextColor(sf::Color::Yellow);
			this->updateTri();
		}
		else
			this->setTextColor(sf::Color::White);
	}

	void SettingsText::onKeyDown(sf::Keyboard::Key keycode) {
		if (keycode == sf::Keyboard::Left && index > 0)
			index--;
		else if (keycode == sf::Keyboard::Right && index < texts.size() - 1)
			index++;
	}

	void SettingsText::updateTri() {
		sf::Vector2f p1(x - grap.getLocalBounds().width / 2 - 2.5, y + grap.getLocalBounds().height / 3 + grap.getLocalBounds().top);//p1(x - grap.getLocalBounds().width / 2 + 5, y + grap.getLocalBounds().height / 3 + 10)
		sf::Vector2f p2(x - grap.getLocalBounds().width / 2 - 2.5, y - grap.getLocalBounds().height / 3 + grap.getLocalBounds().top);
		sf::Vector2f p3(x - grap.getLocalBounds().width / 2 - 2.5 - grap.getLocalBounds().height * sqrt(3) / 3, y + grap.getLocalBounds().top);

		for (int i = 0; i < 3; i++)
			tri_left[i].color = (index > 0) ? sf::Color::White : Global::Gray;
		tri_left[0].position = p1;
		tri_left[1].position = p2;
		tri_left[2].position = p3;

		p1.x += grap.getLocalBounds().width + 10;
		p2.x += grap.getLocalBounds().width + 10;
		p3.x += grap.getLocalBounds().width + 10 + grap.getLocalBounds().height * sqrt(3) * 2 / 3;

		for (int i = 0; i < 3; i++)
			tri_right[i].color = (index < texts.size() - 1) ? sf::Color::White : Global::Gray;
		tri_right[0].position = p1;
		tri_right[1].position = p2;
		tri_right[2].position = p3;
	}

#pragma endregion
}