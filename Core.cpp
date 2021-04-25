#include "Core.hpp"

#define MENU_WITDH 600
#define MENU_HEIGHT 800

namespace rg {

	//global
	Mode Core::now_mode = Mode::MAIN_MENU;
	bool Core::rebuild = true;

	void Core::changeState(Mode new_mode) {
		Core::now_mode = new_mode;
		Core::rebuild = true;
	}

	Mode Core::getNowMode() {
		return Core::now_mode;
	}



	//Core
	Core::Core() : m_renderManager(window) {
		window.create(sf::VideoMode(MENU_WITDH, MENU_HEIGHT), "MainMenu", sf::Style::Close | sf::Style::Titlebar);
		window.setFramerateLimit(60);
		this->m_lastgame_score = 0;
		this->m_highest_score = 0;
	}

	void Core::Run() {
		Core::changeState(Mode::MAIN_MENU);
		while(window.isOpen())
			display();
	}

	void Core::display() {
		//while (window.isOpen()) {
			switch (Core::getNowMode()) {
			case Mode::MAIN_MENU: {
				if (Core::rebuild) {
					Core::rebuild = false;
					if (this->m_mainmenu)
						delete this->m_mainmenu;
					m_mainmenu = new MainMenu(window, m_renderManager);
					m_mainmenu->initMenu();
				}
				m_mainmenu->display();
				break;
			}
			case Mode::GAMING: {
				//edit later
				Game g(window, m_renderManager, BaseData(m_outgame_size, m_ingame_width, m_ingame_height, m_snake_size), 0.001f);
				g.StartGame();
				m_lastgame_score = g.getScore();
				if (m_lastgame_score > m_highest_score)
					m_highest_score = m_lastgame_score;
				break;
			}
			case Mode::GAMEOVER:
				if (Core::rebuild) {
					Core::rebuild = false;
					if (this->m_gameovermenu)
						delete this->m_gameovermenu;
					m_gameovermenu = new GameOverMenu(window, m_renderManager);
					m_gameovermenu->initMenu(this->m_lastgame_score, this->m_highest_score);
				}
				m_gameovermenu->display();
				break;
			}
		//}
	}
}