#include "Core.hpp"

#define MENU_WITDH 600
#define MENU_HEIGHT 800

namespace rg {

	Core::Core() : m_renderManager(window) {
		window.create(sf::VideoMode(MENU_WITDH, MENU_HEIGHT), "MainMenu", sf::Style::Close | sf::Style::Titlebar);
		window.setFramerateLimit(60);
		this->m_lastgame_score = 0;
		this->m_highest_score = 0;
	}

	void Core::Run() {
		Global::C_changeCMode(CMode::MAIN_MENU);
		while(window.isOpen())
			display();
	}

	void Core::display() {
		switch (Global::C_getMode()) {
		case CMode::MAIN_MENU: {
			if (Global::C_Rebuild()) {
				//Core::rebuild = false;
				Global::C_afterRebuild();
				if (this->m_mainmenu)
					delete this->m_mainmenu;
				m_mainmenu = new MainMenu(window, m_renderManager);
				m_mainmenu->initMenu();
			}
			m_mainmenu->display();
			break;
		}
		case CMode::GAMING: {
			if (Global::C_Rebuild()) {
				Global::C_afterRebuild();
				if (this->m_game)
					delete this->m_game;
				m_game = new Game(window, m_renderManager, BaseData(m_outgame_size, m_ingame_width, m_ingame_height, m_snake_size), 0.001f);
			}
			m_game->display();
			break;
		}
		case CMode::GAMEOVER:
			if (Global::C_Rebuild()) {
				Global::C_afterRebuild();
				if (this->m_gameovermenu)
					delete this->m_gameovermenu;
				//score
				m_lastgame_score = m_game->getScore();
				if (m_lastgame_score > m_highest_score)
					m_highest_score = m_lastgame_score;
				//menu
				m_gameovermenu = new GameOverMenu(window, m_renderManager);
				m_gameovermenu->initMenu(this->m_lastgame_score, this->m_highest_score);
			}
			m_gameovermenu->display();
			break;
		}
	}
}