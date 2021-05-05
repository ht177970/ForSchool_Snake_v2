#ifndef SNAKE_RG_AM_H_
#define SNAKE_RG_AM_H_

#include "WindowRender.hpp"
#include "Snake.hpp"
#include "GameObject.hpp"
#include "Food.hpp"
#include "Global.hpp"

namespace rg {
	class BaseAnimation {
	protected:
		BaseAnimation() = default;
		~BaseAnimation() = default;
		sf::RenderWindow* window;
		renderManager* render;
		void handleEvent() {
			sf::Event e;
			while (window->pollEvent(e))
				if (e.type == sf::Event::Closed)
					window->close();
		}
	};
	class AnimationSnake : BaseAnimation {
	public:
		explicit AnimationSnake(sf::RenderWindow& window, renderManager& render, Snake* snake);
		~AnimationSnake() = default;
		void display();
	private:
		const float flashing_interval = 0.2f;
		const float delete_interval = 0.1f;
		const int flash_times = 5;
		int flashed_times = 0;
		float time = 0.f;
		bool show_snake = false;
		sf::Clock clock;
		Snake* snake;
	};

	class AnimationFade : BaseAnimation {
	public:
		explicit AnimationFade(sf::RenderWindow& window, renderManager& render, Wall* snake, Food* food, Background* background);
		~AnimationFade() = default;
		void display();
	private:
		int fade_color = 255;
		Wall* wall;
		Food* food;
		Background* background;
	};
}

#endif