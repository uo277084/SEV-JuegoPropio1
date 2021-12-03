#include "PowerUpVida.h"

PowerUpVida::PowerUpVida(float x, float y, Game* game)
	: Actor("res/icono_recolectable.png", x, y, 20, 20, game) {

	animation = new Animation("res/recolectable.png", width, height,
		256, 32, 5, 8, true, game);
}

void PowerUpVida::update() {
	animation->update();
}

void PowerUpVida::draw(float scrollX) {
	animation->draw(x - scrollX, y);
}