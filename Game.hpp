#ifndef SNAKE_RG_GAME_H_
#define SNAKE_RG_GAME_H_

#include "Snake.hpp"
#include "Food.hpp"
#include "Base.hpp"
#include "Wall.hpp"
#include "Global.hpp"
#include "WindowRender.hpp"
#include "AnimationManager.hpp"
#include <SFML/Graphics.hpp>

namespace rg {

	class Game {
	private:
		sf::RenderWindow* window;
		renderManager* m_renderManager;
		Snake* m_game_snake;
		Food* m_game_food;
		Wall* m_wall;
		AnimationSnake* A1;
		AnimationFade* A2;
		int m_outgame_size, m_ingame_width, m_ingame_height, m_snake_size;
		float m_game_speed;
		int score = 0;
		sf::Clock gameclock;
		float gameTime = 0.0f;
		float move_per_time = 0.1f;
		bool pause = false;
		bool isInFood();
		void AteFood();
		void genFood();
		void handleEvent();
		void displayGaming();
		void displayA1();
		void displayA2();
	public:
		explicit Game(sf::RenderWindow& window, renderManager& render, BaseData data, float game_speed);
		~Game() = default;
		void display();
		int getScore();
	};
}

#endif