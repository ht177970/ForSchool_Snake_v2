#include "AnimationManager.hpp"

namespace rg {
	AnimationSnake::AnimationSnake(sf::RenderWindow& window, renderManager& render, Snake* snake) {
		this->window = &window;
		this->render = &render;
		this->snake = snake;
	}

	void AnimationSnake::display() {
		handleEvent();
		time += clock.restart().asSeconds();
		if (flashed_times < flash_times) {
			if (time > flashing_interval) {
				time = 0.0f;
				show_snake = !show_snake;
				flashed_times++;
			}
		}
		else if (time > delete_interval) {
			time = 0.0f;
			if (!snake->gameover_delete_tail()) {
				Global::G_changeGMode(GMode::A2);
				return;
			}
		}
		render->endGameRender(show_snake);
	}

	AnimationFade::AnimationFade(sf::RenderWindow& window, renderManager& render, Wall* wall, Food* food, Background* background) {
		this->window = &window;
		this->render = &render;
		this->wall = wall;
		this->food = food;
		this->background = background;
	}

	void AnimationFade::display() {
		handleEvent();
		fade_color -= 2;
		if (fade_color < 0)
			fade_color = 0;
		wall->setColor(sf::Color(fade_color, fade_color, fade_color));//White -> Black (255, 255, 255) -> (0, 0, 0)
		food->setColor(sf::Color(fade_color, fade_color, 0));//Yellow -> Black (255, 255, 0) -> (0, 0, 0)
		background->FadeBlack(fade_color);

		render->endGameRender(false);
		if (fade_color == 0)
			Global::C_changeCMode(CMode::GAMEOVER);
	}
}