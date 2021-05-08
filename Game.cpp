#include "Game.hpp"
#include "Core.hpp"
#include <iostream>
#include <cstring>
#include <random>

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

	Background::Background(BaseData data) {
		int width = data.outgame_size * 2 + data.ingame_width, height = data.outgame_size * 2 + data.ingame_height;
		grap.setSize(sf::Vector2f(width, height));
		grap.setFillColor(Global::settings.getNewColor());//sf::Color::Color(178, 255, 102)
	}

	void Background::setColor(sf::Color new_color) {
		grap.setFillColor(new_color);
	}

	sf::Color Background::getColor() {
		return grap.getFillColor();
	}

	void Background::FadeBlack(int _color) {
		sf::Color now_color = this->getColor();
		int color[] = { now_color.r, now_color.g , now_color.b };
		int max = 0;
		for (int i = 0; i < 3; i++) {
			if ((color[i] -= 2) < 0)
				color[i] = 0;
			if (color[i] > max)
				max = color[i];
		}
		if(max + 10 > _color)
			this->setColor(sf::Color(color[0], color[1], color[2]));
	}

	void Background::draw(sf::RenderWindow& window) {
		window.draw(grap);
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

		this->m_game_snake = new Snake(data);
		this->m_game_food = new Food(data);
		this->m_wall = new Wall(data);
		this->m_background = new Background(data);

		this->m_renderManager->clearAllGraphics();
		this->m_renderManager->addGraphics(this->m_background);
		this->m_renderManager->addGraphics(this->m_wall);
		this->m_renderManager->addGraphics(this->m_game_food);
		this->m_renderManager->addGraphics(this->m_game_snake);
		this->m_renderManager->setSanke(m_game_snake);

		this->A1 = new AnimationSnake(_window, render, m_game_snake);
		this->A2 = new AnimationFade(_window, render, m_wall, m_game_food, m_background);

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
		if(score % Global::settings.getCCperFood() == 0)
			m_background->setColor(Global::settings.getNewColor());
		//std::random_device random;
		//std::mt19937 generator(random());
		//sf::Color(generator() % 256, generator() % 256, generator() % 256)
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