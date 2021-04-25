#include "Game.hpp"
#include "Core.hpp"
#include "Wall.hpp"
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



	Game::Game(sf::RenderWindow& _window, renderManager& render, BaseData data, float game_speed) {
		this->window = &_window;
		this->m_renderManager = &render;
		this->m_outgame_size = data.outgame_size;
		this->m_ingame_width = data.ingame_width;
		this->m_ingame_height = data.ingame_height;
		this->m_snake_size = data.snake_size;
		this->m_game_speed = game_speed;

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

		this->A1 = new AnimationSnake(_window, render, m_game_snake);
		this->A2 = new AnimationFade(_window, render, m_wall, m_game_food);

		Global::G_changeGMode(GMode::Gaming);
	}

	void Game::handleEvent() {
		sf::Event e;
		while (window->pollEvent(e)) {
			switch (e.type)
			{
			case sf::Event::Closed:
				window->close();
				return;
			case sf::Event::KeyPressed:
				if (e.key.code == sf::Keyboard::P)//invisable
					pause = !pause;
				break;
			}
		}
	}

	void Game::display() {
		this->handleEvent();
		switch (Global::G_getMode()) {
		case GMode::Gaming:
			displayGaming();
			break;
		case GMode::A1:
			displayA1();
			break;
		case GMode::A2:
			displayA2();
			break;
		}
	}

	void Game::displayGaming() {
		if (!pause) {
			m_game_snake->detectWayKeys();

			this->gameTime += gameclock.restart().asSeconds();
			if (this->gameTime > this->move_per_time)
			{
				this->gameTime = 0.f;
				if (!m_game_snake->move()) {
					Global::G_changeGMode(GMode::A1);
					return;
				}
				if (this->isInFood()) {
					this->AteFood();
					this->move_per_time -= m_game_speed;
				}
			}
			m_renderManager->Render();
		}
		else
			gameclock.restart();
	}

	void Game::displayA1() {
		A1->display();
	}

	void Game::displayA2() {
		A2->display();
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

	int Game::getScore() {
		return this->score;
	}
}