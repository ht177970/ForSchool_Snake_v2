#include "Snake.hpp"
#include <iostream>

namespace rg {

#pragma region Unit functions

	Unit::Unit(Unit* previous_unit, int X, int Y, int size) : Unit(previous_unit, X, Y) {
		//grap.setRadius(size / 2);
		grap.setSize(sf::Vector2f(size, size));
		grap.setFillColor(sf::Color::Cyan);
	}

	Unit::Unit(Unit* previous_unit, int X, int Y) {
		u1 = previous_unit;
		this->setPos({ X, Y });
	}

	void Unit::draw(sf::RenderWindow& window) {
		window.draw(grap);
	}

	void Unit::setPos(Pos pos) {
		x = pos.x;
		y = pos.y;
		grap.setPosition(x, y);
	}

	void Unit::setNext(Unit* next_unit) {
		u2 = next_unit;
	}

	HeadUnit::HeadUnit(Unit* previous_unit, int X, int Y, int size) : Unit(previous_unit, X, Y) {
		u1 = previous_unit;
		this->setPos({ X, Y });
		m_size = size;
		c_adjust_x = 0;
		c_adjust_y = 0;
		m_grap_rect.setSize(sf::Vector2f(m_size, m_size /2));
		m_grap_rect.setFillColor(sf::Color::Color(0, 139, 139));
		m_grap_circle.setRadius(m_size / 2);
		m_grap_circle.setFillColor(sf::Color::Color(0, 139, 139));
		first_draw = true;
	}

	void HeadUnit::draw(sf::RenderWindow& window) {
		if (!first_draw)
			window.draw(m_grap_rect);
		else
			first_draw = false;
		window.draw(m_grap_circle);
	}

	void HeadUnit::setPos(Pos pos) {
		x = pos.x;
		y = pos.y;
		m_grap_rect.setPosition(x + c_adjust_x, y + c_adjust_y);
		m_grap_circle.setPosition(x, y);
	}

	void HeadUnit::onWayChanged(Snake::Ways new_direction) {
		switch (new_direction)
		{
		case Snake::Ways::UP:
			m_grap_rect.setSize(sf::Vector2f(m_size, m_size / 2));
			c_adjust_x = 0;
			c_adjust_y = m_size / 2;
			break;
		case Snake::Ways::DOWN:
			m_grap_rect.setSize(sf::Vector2f(m_size, m_size / 2));
			c_adjust_x = 0;
			c_adjust_y = 0;
			break;
		case Snake::Ways::R:
			m_grap_rect.setSize(sf::Vector2f(m_size / 2, m_size));
			c_adjust_x = 0;
			c_adjust_y = 0;
			break;
		case Snake::Ways::L:
			m_grap_rect.setSize(sf::Vector2f(m_size / 2, m_size));
			c_adjust_x = m_size / 2;
			c_adjust_y = 0;
			break;
		default:
			break;
		}
	}

	/*Unit* Unit::next() {
		return u2;
	}*/
#pragma endregion

	Snake::Snake(int* outgame_size, int* ingame_width, int* ingame_height, int SIZE) {
		m_outgame_size = outgame_size;
		m_ingame_width = ingame_width;
		m_ingame_height = ingame_height;
		x = *m_ingame_width / 2;
		y = *m_ingame_height / 2;
		m_tail_x = x;
		m_tail_y = y;
		size = SIZE;
		head = new HeadUnit(nullptr, x, y, size);//head's prev is nullptr
		tail = head;
		now_direction = Ways::Non;
		last_move_direction = Ways::Non;
	}

	void Snake::setDirection(Ways new_direction) {
		if (static_cast<int>(new_direction) + static_cast<int>(last_move_direction))
			now_direction = new_direction;
	}

	bool Snake::detectWayKeys() {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			this->setDirection(Snake::Ways::UP);
			return true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			this->setDirection(Snake::Ways::DOWN);
			return true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			this->setDirection(Snake::Ways::R);
			return true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			this->setDirection(Snake::Ways::L);
			return true;
		}
		return false;
	}

	bool Snake::move() {
		switch (now_direction) {
		case Ways::UP:
			y -= size;
			break;
		case Ways::DOWN:
			y += size;
			break;
		case Ways::L:
			x -= size;
			break;
		case Ways::R:
			x += size;
			break;
		}
		if (now_direction != last_move_direction)
			dynamic_cast<HeadUnit*>(head)->onWayChanged(now_direction);
		last_move_direction = now_direction;
		if (illegalPos())//edit later
			return false;
		Unit* moving = tail;
		Pos last_position = tail->getPos();
		m_tail_x = last_position.x;
		m_tail_y = last_position.y;
		while (moving->prev()) {
			moving->setPos(moving->prev()->getPos());
			moving = moving->prev();
		}
		head->setPos({ x, y });
		return !this->posInBody({ x,y });
	}

	bool Snake::illegalPos() {
		return (x < *m_outgame_size || y < *m_outgame_size || x > *m_outgame_size + *m_ingame_width - size || y > *m_outgame_size + *m_ingame_height - size);
	}

	bool Snake::posInBody(Pos position) {
		Unit* testing = tail;
		while (testing->prev()) {
			if (testing->getPos() == position)
				return true;
			testing = testing->prev();
		}
		return false;
	}

	bool Snake::wait_start(sf::RenderWindow& window) {
		sf::Event e;
		while (window.isOpen()) {
			while (window.pollEvent(e))
				if (e.type == sf::Event::Closed) {
					window.close();
					return false;
				}
			if (this->detectWayKeys())
				return true;
		}
		return false;
	}

	void Snake::draw(sf::RenderWindow& window) {
		Unit* drawing = tail;
		while (drawing->prev()) {
			drawing->draw(window);
			drawing = drawing->prev();
		}
		head->draw(window);
	}

	void Snake::gainFood() {
		Unit* prev_tail = tail;
		tail = new Unit(tail, this->getTailX_prev(), this->getTailY_prev(), size);
		prev_tail->setNext(tail);
	}

	bool Snake::gameover_delete_tail() {
		Unit* new_tail = tail->prev();
		delete tail;
		tail = new_tail;
		return tail;
	}

	Unit::~Unit() {
		
	}

	Snake::~Snake() {
		
	}
}