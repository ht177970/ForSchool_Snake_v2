#ifndef SNAKE_RG_GAME_H_
#define SNAKE_RG_GAME_H_

#include "Snake.hpp"
#include "Food.hpp"
#include "Base.hpp"
#include "WindowRender.hpp"
#include <SFML/Graphics.hpp>

namespace rg {

	class Wall : public BaseDrawable {
	public:
		explicit Wall(BaseData data);
		~Wall() = default;
		void draw(sf::RenderWindow& window) override;
		void setColor(sf::Color new_color);
	private:
		int m_outgame_size, m_ingame_width, m_ingame_height, m_snake_size;
		sf::Color m_color;
	};

	class Game {
	private:
		sf::RenderWindow* window;
		renderManager* m_renderManager;
		Snake* m_game_snake;
		Food* m_game_food;
		Wall* m_wall;
		int m_outgame_size, m_ingame_width, m_ingame_height, m_snake_size;
		int score;
		float m_game_speed;
		bool pause = false;
		void GameOverSnakeAnimation();
		void GameOverFadeAnimation();
		void GameLoop();
		bool isInFood();
		void AteFood();
		void genFood();
	public:
		explicit Game(sf::RenderWindow& window, renderManager& render, BaseData data, float game_speed);
		~Game() = default;
		void StartGame();
		int getScore();
	};
}

#endif