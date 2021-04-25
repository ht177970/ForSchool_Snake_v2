#ifndef SNAKE_RG_WindowRender_H_
#define SNAKE_RG_WindowRender_H_

#include <vector>
#include "Base.hpp"
#include "Snake.hpp"

namespace rg {
	class renderManager {
	public:
		renderManager(sf::RenderWindow& window);
		~renderManager() = default;
		void setSanke(Snake* snake);
		void Render();
		void endGameRender(bool show_snake);
		void addGraphics(BaseDrawable* grap);
		void clearAllGraphics();
		bool startGame();
	private:
		std::vector<BaseDrawable*> m_AllGraphics;
		Snake* m_snake;
		sf::RenderWindow* window;
	};
}

#endif