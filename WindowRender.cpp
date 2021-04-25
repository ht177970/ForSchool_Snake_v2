#include "WindowRender.hpp"

namespace rg {
	renderManager::renderManager(sf::RenderWindow& window) {
		this->window = &window;
		this->m_snake = nullptr;
	}

	void renderManager::setSanke(Snake* snake) {
		this->m_snake = snake;
	}

	void renderManager::addGraphics(BaseDrawable* grap) {
		m_AllGraphics.push_back(grap);
	}

	void renderManager::clearAllGraphics() {
		for (auto grap : m_AllGraphics)
			if(grap)
				delete grap;
		m_AllGraphics.clear();
	}
	
	void renderManager::Render() {
		window->clear();
		for (auto& grap : m_AllGraphics)
			grap->draw(*window);
		window->display();
	}

	void renderManager::endGameRender(bool show_snake) {
		window->clear();
		for (auto& grap : m_AllGraphics)
			if (show_snake || grap != m_snake)
				grap->draw(*window);
		window->display();
	}

	bool renderManager::startGame() {
		this->Render();
		return m_snake->wait_start(*window);
	}
}