#ifndef SNAKE_RG_CORE_H_
#define SNAKE_RG_CORE_H_

#include "WindowRender.hpp"
#include "Game.hpp"
#include "Menu.hpp"
#include "Global.hpp"

namespace rg {
	class Core {
	public:
		explicit Core();
		~Core() = default;
		void Run();
	private:
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