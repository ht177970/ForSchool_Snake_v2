#ifndef SNAKE_RG_SNAKE_H_
#define SNAKE_RG_SNAKE_H_

#include "Base.hpp"
#include <SFML/Graphics.hpp>

namespace rg {

	class Unit : BaseDrawable{
	public:
		explicit Unit(Unit* previous_unit, int X, int Y, int size);
		~Unit();
		void setNext(Unit* next_unit);
		void draw(sf::RenderWindow& window) override;
		virtual void setPos(Pos pos);
		Unit* prev() { return u1; }
		Unit* next() { return u2; }
		Pos getPos() { return { x,y }; }
	protected:
		int x, y;
		Unit* u1, * u2;
		explicit Unit(Unit* previous_unit, int X, int Y);
	private:
		sf::RectangleShape grap;
	};

	class Snake : public BaseDrawable{
	public:
		enum class Ways { UP = 1, DOWN = -1, R = 2, L = -2, Non = 0};//R = Right, L = Left
		explicit Snake(int* outgame_size, int* ingame_width, int* ingame_height, int SIZE);
		~Snake();
		bool move();
		int getHeadX() { return x; };
		int getHeadY() { return y; };
		void draw(sf::RenderWindow& window) override;
		void gainFood();
		bool detectWayKeys();
		bool posInBody(int x, int y) { return posInBody({ x,y }); }
		bool posInBody(Pos position);
		bool wait_start(sf::RenderWindow& window);
		bool gameover_delete_tail();
	private:
		int x, y;
		int size;
		int m_tail_x, m_tail_y;
		int* m_outgame_size, * m_ingame_width, * m_ingame_height;
		Ways now_direction, last_move_direction;
		Unit* head;
		Unit* tail;
		int getTailX_prev() { return m_tail_x; }
		int getTailY_prev() { return m_tail_y; }
		bool illegalPos();
		void setDirection(Ways way);
	};

	class HeadUnit : public Unit {
	public:
		explicit HeadUnit(Unit* previous_unit, int X, int Y, int size);
		void draw(sf::RenderWindow& window) override;
		void setPos(Pos pos) override;
		void onWayChanged(Snake::Ways new_direction);
	private:
		int m_size;
		int c_adjust_x, c_adjust_y;
		bool first_draw;
		sf::RectangleShape m_grap_rect;
		sf::CircleShape m_grap_circle;
	};
}

#endif
