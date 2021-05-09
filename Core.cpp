#include "Core.hpp"
#include "DataManager.hpp"
#include <fstream>

#define MENU_WITDH 600
#define MENU_HEIGHT 800

namespace rg {

	Core::Core(sf::Font g_font, sf::Texture g_pic) : m_renderManager(window) {
		window.create(sf::VideoMode(MENU_WITDH, MENU_HEIGHT), "MainMenu", sf::Style::Close | sf::Style::Titlebar);
		window.setFramerateLimit(60);
		window.setKeyRepeatEnabled(false);
		this->m_lastgame_score = 0;
		this->m_highest_score = GameData::getHighestScore();
		this->m_mainmenu = nullptr;
		this->m_game = nullptr;
		this->m_gameovermenu = nullptr;
		this->m_settingsmenu = nullptr;

		Global::settings.setFont(g_font);
		Global::settings.setPic(g_pic);
	}

	void Core::Run() {
		Global::C_changeCMode(CMode::MAIN_MENU);
		while(window.isOpen())
			display();
	}

	void Core::display() {
		switch (Global::C_getMode()) {
		case CMode::MAIN_MENU:
			display_MainMenu();
			break;
		case CMode::GAMING:
			display_Game();
			break;
		case CMode::GAMEOVER:
			display_GameOver();
			break;
		case CMode::SETTINGS:
			display_Settings();
			break;
		}
	}

	void Core::display_MainMenu() {
		if (Global::C_Rebuild()) {
			if (this->m_mainmenu)
				delete this->m_mainmenu;
			m_mainmenu = new MainMenu(window, m_renderManager);
			m_mainmenu->initMenu();
		}
		m_mainmenu->display();
	}

	void Core::display_Game() {
		if (Global::C_Rebuild()) {
			if (this->m_game)
				delete this->m_game;
			m_game = new Game(window, m_renderManager, BaseData(Global::settings.getOutGameSize(), Global::settings.getInGameWidth(),
				Global::settings.getInGameHeight(), Global::settings.getSnakeSize()), 0.001f);
		}
		m_game->display();
	}

	void Core::display_GameOver() {
		if (Global::C_Rebuild()) {
			//Global::C_afterRebuild();
			if (this->m_gameovermenu)
				delete this->m_gameovermenu;
			//score
			m_lastgame_score = m_game->getScore();
			if (m_lastgame_score > m_highest_score) {
				m_highest_score = m_lastgame_score;
				GameData::saveHighestScore(m_highest_score);
			}
			//menu
			m_gameovermenu = new GameOverMenu(window, m_renderManager);
			m_gameovermenu->initMenu(this->m_lastgame_score, this->m_highest_score);
		}
		m_gameovermenu->display();
	}

	void Core::display_Settings() {
		if (Global::C_Rebuild()) {
			if (this->m_settingsmenu)
				delete this->m_settingsmenu;
			m_settingsmenu = new SettingsMenu(window, m_renderManager);
			m_settingsmenu->initMenu();
		}
		m_settingsmenu->display();
	}
}