#include "Game.hpp"
#include "Core.hpp"
#include <iostream>
#include <cstring>

namespace rg {

	Wall::Wall(BaseData data) {
		this->m_outgame_size = data.outgame_size;
		this->m_ingame_width = data.ingame_width;
		this->m_ingame_height = data.ingame_height;
		this->m_snake_size = data.snake_size;
		this->m_color = sf::Color::White;
	}

	void Wall::setColor(sf::Color new_color) {
		m_color = new_color;
	}

	void Wall::draw(sf::RenderWindow& window) {
		int col = m_ingame_width / m_snake_size;
		int row = m_ingame_height / m_snake_size;
		/*for (int i = 0; i <= row; i++) {
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(m_outgame_size, m_outgame_size + m_snake_size * i)),
				sf::Vertex(sf::Vector2f(m_outgame_size + m_ingame_width, m_outgame_size + m_snake_size * i))
			};
			window->draw(line, 2, sf::Lines);
		}
		for (int i = 0; i <= col; i++) {
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(m_outgame_size + m_snake_size * i, m_outgame_size)),
				sf::Vertex(sf::Vector2f(m_outgame_size + m_snake_size * i, m_outgame_size + m_ingame_height))
			};
			window->draw(line, 2, sf::Lines);
		}*/
		for (int i : {0, col}) {
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(m_outgame_size + m_snake_size * i, m_outgame_size), m_color),
				sf::Vertex(sf::Vector2f(m_outgame_size + m_snake_size * i, m_outgame_size + m_ingame_height), m_color)
			};
			window.draw(line, 2, sf::Lines);
		}
		for (int i : {0, row}) {
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(m_outgame_size, m_outgame_size + m_snake_size * i), m_color),
				sf::Vertex(sf::Vector2f(m_outgame_size + m_ingame_width, m_outgame_size + m_snake_size * i), m_color)
			};
			window.draw(line, 2, sf::Lines);
		}
	}


	Game::Game(sf::RenderWindow& _window, renderManager& render, BaseData data, float game_speed) {// :
		/*m_renderManager(window),*/ //m_wall(data) {
		this->window = &_window;
		this->m_renderManager = &render;
		this->m_outgame_size = data.outgame_size;
		this->m_ingame_width = data.ingame_width;
		this->m_ingame_height = data.ingame_height;
		this->m_snake_size = data.snake_size;
		this->m_game_speed = game_speed;
		this->score = 0;

		int width = m_outgame_size * 2 + m_ingame_width, height = m_outgame_size * 2 + m_ingame_height;
		this->window->setSize(sf::Vector2u(width, height));
		this->window->setView(sf::View(sf::FloatRect(0, 0, width, height)));
		this->window->setTitle("Snake Game");

		this->m_game_snake = new Snake(&m_outgame_size, &m_ingame_width, &m_ingame_height, m_snake_size);
		this->m_game_food = new Food(m_outgame_size, m_ingame_width, m_ingame_height, m_snake_size);
		this->m_wall = new Wall(data);

		this->m_renderManager->clearAllGraphics();
		this->m_renderManager->addGraphics(this->m_wall);
		this->m_renderManager->addGraphics(this->m_game_food);
		this->m_renderManager->addGraphics(this->m_game_snake);
		this->m_renderManager->setSanke(m_game_snake);
	}

	void Game::StartGame() {
		if(m_renderManager->startGame())
			GameLoop();
		else
			this->~Game();
	}

	void Game::GameLoop() {
		sf::Clock gameClock;
		float elapsedGameTime = 0.0f;
		float timeStep = 0.1f;
		sf::Event e;
		while (window->isOpen()) {
			while (window->pollEvent(e)) {
				switch (e.type)
				{
				case sf::Event::Closed:
					window->close();
					return;
				case sf::Event::KeyPressed:
					if (e.key.code == sf::Keyboard::P) {//edit later
						if (!pause)
							elapsedGameTime += gameClock.restart().asSeconds();
						pause = !pause;
					}
					break;
				}
			}
			if (!pause) {
				m_game_snake->detectWayKeys();

				elapsedGameTime += gameClock.restart().asSeconds();
				if (elapsedGameTime > timeStep)
				{
					//elapsedGameTime = (static_cast<int>(elapsedGameTime * 100) % static_cast<int>(timeStep * 100)) / 100.0f;
					elapsedGameTime = 0.f;
					if (!m_game_snake->move()) {
						this->GameOverSnakeAnimation();
						return;
					}
					if (this->isInFood()) {
						this->AteFood();
						timeStep -= m_game_speed;
					}
				}
				m_renderManager->Render();
			}
			else
				gameClock.restart();
		}
	}

	bool Game::isInFood() {
		return (m_game_snake->getHeadX() == m_game_food->getX() && m_game_snake->getHeadY() == m_game_food->getY());
	}

	void Game::AteFood() {
		score++;
		this->genFood();
		m_game_snake->gainFood();
	}

	void Game::genFood() {
		do {
			m_game_food->generateNewPosition();
		} while (m_game_snake->posInBody(m_game_food->getX(), m_game_food->getY()));
	}
	
	void Game::GameOverSnakeAnimation() {
		const float flashing_interval = 0.2f;
		const float delete_interval = 0.1f;

		sf::Clock gameClock;
		int _times = 0;
		bool show = false; 
		float time = 0.0f;
		sf::Event e;
		while (window->isOpen()) {
			while (window->pollEvent(e))
				if (e.type == sf::Event::Closed) {
					window->close();
					return;
				}
			time += gameClock.restart().asSeconds();
			if (_times < 5){//flash 5 times
				if (time > flashing_interval) {
					time = 0.0f;
					show = !show;
					_times++;
				}
			}
			else if (time > delete_interval){
				time = 0.0f;
				if (!m_game_snake->gameover_delete_tail())
					break;
			}
			m_renderManager->endGameRender(show);
		}
		this->GameOverFadeAnimation();
	}

	void Game::GameOverFadeAnimation() {
		sf::Event e;
		int fade_color = 255;
		while (window->isOpen()) {
			while (window->pollEvent(e))
				if (e.type == sf::Event::Closed) {
					window->close();
					return;
				}

			fade_color -= 2;
			if (fade_color < 0)
				fade_color = 0;
			m_wall->setColor(sf::Color(fade_color, fade_color, fade_color));//White -> Black (255, 255, 255) -> (0, 0, 0)
			m_game_food->setColor(sf::Color(fade_color, fade_color, 0));//Yellow -> Black (255, 255, 0) -> (0, 0, 0)

			m_renderManager->endGameRender(false);
			if (fade_color == 0)
				break;
		}
		Core::changeState(Mode::GAMEOVER);
	}

	int Game::getScore() {
		return this->score;
	}
}