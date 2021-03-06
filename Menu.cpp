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
			if (t->isPosIn(sf::Vector2i(mouse.x, mouse.y))) {
				changeTextIndex(t->getId());
				break;
			}
	}

	void ClickableMenu::onMouseClick() {
		sf::Vector2i mousepos = sf::Mouse::getPosition(*window);
		if (this->getBaseClickable()[m_text_index]->isPosIn(mousepos))
			EnterPressed(m_text_index);
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

		sf::String texts[] = { L"?}?l?C??", L"?C???]?w", L"???}" };
		Pos position = Pos(300, 300);
		int next_y = 120;

		Text* help1 = new Text(L"????  ????", Pos(49, 766));
		Text* help2 = new Text(L"Enter?T?{", Pos(49, 786));
		help1->setTextSize(20U);
		help2->setTextSize(20U);
		m_renderManager->addGraphics(help1);
		m_renderManager->addGraphics(help2);

		for (int i = 0; i < 3; i++) {
			auto tmp = new Text(texts[i], position.AddY(next_y));
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
		ss << L" ?A?o???o???F " << score << L" ??!";

		Pos position = Pos(Global::settings.getIngameScreenWidth() / 2, Global::settings.getIngameScreenHeight() / 4);
		unsigned int text_size;
		text_size = static_cast<unsigned int>(90.f * position.x / 500);

		Text* now_score_t = new Text(ss.str(), position);//x = 500(1000), y = 200(800)
		now_score_t->setTextSize(text_size);//90(1000)
		now_score_t->setTextColor(sf::Color(0, 255, 0));
		m_renderManager->addGraphics(now_score_t);//color = lime

		ss.str(L"");
		ss << L"???v???? " << highest_score << L" ??";
		Text* highest_score_t = new Text(ss.str(), Pos(position.x, position.y + text_size));
		text_size = static_cast<unsigned int>(30.f * position.x / 500);
		highest_score_t->setTextSize(text_size);
		highest_score_t->setTextColor(sf::Color(143, 188, 143));
		m_renderManager->addGraphics(highest_score_t);//color = orange

		sf::String texts[] = { L"?A???@??", L"?^?D????", L"???}" };
		position.y += text_size * 4;
		text_size = static_cast<unsigned int>(60.f * position.x / 500);
		for (int i = 0; i < 3; i++) {
			auto tmp = new Text(texts[i], position.AddY(text_size * 2));
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

		sf::String texts[] = { L"?C??????????", L"?C???????W?v"};
		std::vector<sf::String> subs[] = { 
			{L"????", L"?L??", L"?`??", L"?m??"},
			{L"??", L"?C", L"??"}
		};
		int default_index[] = { Global::settings.getColorMode(), Global::settings.getColorF_index()};
		Pos position = Pos(150, 200);
		int next_y = 120;

		Text* help1 = new Text(L"????  ????", Pos(49, 766));
		Text* help2 = new Text(L"Enter?T?{", Pos(49, 786));
		help1->setTextSize(20U);
		help2->setTextSize(20U);
		m_renderManager->addGraphics(help1);
		m_renderManager->addGraphics(help2);

		int id = 0;

		SettingsText* saveSettings = new SettingsText({ L"?x?s?????^" }, Pos(300, 600), 0);
		saveSettings->setTextIndexPointer(&m_text_index);
		saveSettings->setId(id++);
		s_clickable_texts.push_back(saveSettings);

		SettingsText* n_saveSettings = new SettingsText({ L"???x?s???^" }, Pos(300, 700), 0);
		n_saveSettings->setTextIndexPointer(&m_text_index);
		n_saveSettings->setId(id++);
		s_clickable_texts.push_back(n_saveSettings);

		for (int i = 0; i < 2; i++) {
			auto settings_name = new Text(texts[i], position.AddY(next_y));
			settings_name->setTextSize(30U);
			settings_name->setTextColor(sf::Color::Black);
			m_renderManager->addGraphics(settings_name);

			auto settings_choose = new SettingsText(subs[i], Pos(position.x + 200, position.y), default_index[i]);
			settings_choose->setId(id + i);
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
		switch (index) {
		case 0://save
			Global::settings.setColorMode(s_clickable_texts[2]->getSubIndex());//2 = color settings
			Global::settings.setColorF(s_clickable_texts[3]->getSubIndex());//3 = color change per second
		case 1://dont save
			Global::C_changeCMode(CMode::MAIN_MENU);
			break;
		default:
			SettingsText* clickedText = s_clickable_texts[index];
			clickedText->onKeyDown(clickedText->getVirtualKey());
			break;
		}
	}

	void SettingsMenu::otherKeyDown(sf::Keyboard::Key keycode) {
		if (keycode == sf::Keyboard::Left || keycode == sf::Keyboard::Right)
			s_clickable_texts[m_text_index]->onKeyDown(keycode);
	}


#pragma endregion

#pragma region objects

	Text::Text(sf::String text, Pos position) {
		this->m_id = -1;//default
		this->m_font = Global::settings.getFont();
		this->grap = sf::Text(text, m_font, 60U);//default
		this->grap.setOrigin(grap.getLocalBounds().width / 2, grap.getLocalBounds().height / 2);
		this->grap.setPosition(position.x, position.y);
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
		this->TextChanged();
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

	bool Text::isPosIn(sf::Vector2i pos) {
		sf::FloatRect grap_rect = grap.getLocalBounds();
		pos.x -= grap.getPosition().x + grap_rect.left - grap_rect.width / 2;
		pos.y -= grap.getPosition().y + grap_rect.top - grap_rect.height / 2;
		return 0 <= pos.x && pos.x <= grap_rect.width &&
			0 <= pos.y && pos.y <= grap_rect.height;
	}

	MainImage::MainImage() {
		texture = Global::settings.getPic();
		sprite.setTexture(texture);
		sprite.setPosition(0, 0);
	}

	void MainImage::draw(sf::RenderWindow& window) {
		window.draw(sprite);
	}

	SettingsText::SettingsText(std::vector<sf::String> texts, Pos position, int default_index) :
		Text(texts[default_index], position),
		tri_left(sf::Triangles, 3), 
		tri_right(sf::Triangles, 3) 
	{

		this->texts = texts;
		this->x = position.x;
		this->y = position.y;
		index = default_index;
		tri_visible_left = false;
		tri_visible_right = false;
		virtual_key = sf::Keyboard::Unknown;
		updateTri();
	}

	void SettingsText::TextChanged() {
		this->updateTri();
	}

	void SettingsText::draw(sf::RenderWindow& window) {
		Text::draw(window);
		if (*Text::index == this->m_id && texts.size() > 1) {
			window.draw(tri_left);
			window.draw(tri_right);
		}
	}

	void SettingsText::updateText() {
		this->setTextColor((*Text::index == this->m_id) ? sf::Color::Yellow : sf::Color::White);
	}

	void SettingsText::onKeyDown(sf::Keyboard::Key keycode) {
		if (keycode == sf::Keyboard::Left && index > 0)
			index--;
		else if (keycode == sf::Keyboard::Right && index < texts.size() - 1)
			index++;
		grap.setString(texts[index]);
		grap.setOrigin(grap.getLocalBounds().width / 2, grap.getLocalBounds().height / 2);
		this->updateTri();
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

	bool SettingsText::isPosIn(sf::Vector2i pos) {
		this->virtual_key = sf::Keyboard::Unknown;
		if (Text::isPosIn(pos))
			return true;
		if (*Text::index == this->m_id && texts.size() > 1) {
			return this->PosInTri(pos);
		}
		return false;
	}

	bool SettingsText::PosInTri(Pos pos) {
		Vec2d vec(pos);
		//left
		vec.setA(tri_left[0].position);
		vec.setB(tri_left[1].position);
		vec.setC(tri_left[2].position);
		if (vec.PointInTri()) {
			this->virtual_key = sf::Keyboard::Left;
			return true;
		}
		//right
		vec.setA(tri_right[0].position);
		vec.setB(tri_right[1].position);
		vec.setC(tri_right[2].position);
		if (vec.PointInTri()) {
			this->virtual_key = sf::Keyboard::Right;
			return true;
		}
		return false;
	}

	sf::Keyboard::Key SettingsText::getVirtualKey() {
		return this->virtual_key;
	}

#pragma endregion
}