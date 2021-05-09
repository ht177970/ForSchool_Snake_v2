#ifndef SNAKE_RG_BASE_H_
#define SNAKE_RG_BASE_H_

#include <SFML/Graphics.hpp>

namespace rg {
	class BaseDrawable {
	public:
		virtual ~BaseDrawable() = default;
		virtual void draw(sf::RenderWindow& window) = 0;
	};

	struct Pos {
		float x, y;

		Pos(int x, int y) {
			this->x = x;
			this->y = y;
		}

		Pos(float x, float y) {
			this->x = x;
			this->y = y;
		}

		Pos(sf::Vector2f v2f) {
			this->x = v2f.x;
			this->y = v2f.y;
		}

		Pos(sf::Vector2i v2i) {
			this->x = v2i.x;
			this->y = v2i.y;
		}

		Pos AddX(float delta) {
			this->x += delta;
			return *this;
		}

		Pos AddY(float delta) {
			this->y += delta;
			return *this;
		}

		bool operator==(Pos other) {
			return (x == other.x && y == other.y);
		}
	};

	struct Vectors {
		bool sAB, sBC, sCA;

		//向量外積公式(x1y2 - x2y1)
		Vectors(Pos PA, Pos PB, Pos PC) {
			sAB = ((PA.x * PB.y) - (PB.x * PA.y)) > 0.0f;
			sBC = ((PB.x * PC.y) - (PC.x * PB.y)) > 0.0f;
			sCA = ((PC.x * PA.y) - (PA.x * PC.y)) > 0.0f;
		}

		bool sameSign() {
			return (sAB == sBC && sBC == sCA);
		}
	};

	struct Vec2d {
		//點p
		Pos p;
		//向量PA PB PC
		Pos* v_PA, * v_PB, * v_PC;

		Vec2d(Pos P) : p(P.x, P.y) {
			v_PA = nullptr;
			v_PB = nullptr;
			v_PC = nullptr;
		}

		void setA(Pos pos) {
			v_PA = new Pos(pos.x - p.x, pos.y - p.y);
		}

		void setB(Pos pos) {
			v_PB = new Pos(pos.x - p.x, pos.y - p.y);
		}

		void setC(Pos pos) {
			v_PC = new Pos(pos.x - p.x, pos.y - p.y);
		}

		bool PointInTri() {
			if (v_PA && v_PB && v_PC) {
				Vectors v(*v_PA, *v_PB, *v_PC);
				return v.sameSign();
			}
			return false;
		}
	};

	struct BaseData {
		int outgame_size, ingame_width, ingame_height, snake_size;
		BaseData(int outgame_size, int ingame_width, int ingame_height, int snake_size) {
			this->outgame_size = outgame_size;
			this->ingame_width = ingame_width;
			this->ingame_height = ingame_height;
			this->snake_size = snake_size;
		}
	};
}

#endif