#ifndef SNAKE_RG_CORE_H_
#define SNAKE_RG_CORE_H_

#include "WindowRender.hpp"
#include "Game.hpp"
#include "Menu.hpp"
#include "Global.hpp"

namespace rg {
	class Core {
	public:
		explicit Core(sf::Font g_font, sf::Texture g_pic);
		~Core() = default;
		void Run();
	private:
		int m_lastgame_score;
		int m_highest_score;
		sf::RenderWindow window;
		renderManager m_renderManager;
		MainMenu* m_mainmenu = nullptr;
		Game* m_game = nullptr;
		GameOverMenu* m_gameovermenu = nullptr;
		void display();
	};
}

#endif