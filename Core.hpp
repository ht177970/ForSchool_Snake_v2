#ifndef SNAKE_RG_CORE_H_
#define SNAKE_RG_CORE_H_

#include "WindowRender.hpp"
#include "Game.hpp"
#include "Menu.hpp"

namespace rg {
	static enum class Mode {
		MAIN_MENU, GAMING, SETTINGS, GAMEOVER, NONE
	};
	class Core {
	public:
		static void changeState(Mode new_mode);
		static Mode getNowMode();

		explicit Core();
		~Core() = default;
		void Run();
	private:
		static Mode now_mode;
		static bool rebuild;

		int m_outgame_size = 100, m_ingame_width = 800, m_ingame_height = 600, m_snake_size = 20;
		int m_lastgame_score;
		int m_highest_score;
		sf::RenderWindow window;
		renderManager m_renderManager;
		MainMenu* m_mainmenu;
		Game* m_game;
		GameOverMenu* m_gameovermenu;
		void display();
	};
}

#endif