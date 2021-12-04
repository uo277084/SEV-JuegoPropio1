#include "PowerUpVida.h"

PowerUpVida::PowerUpVida(float x, float y, Game* game)
	: Actor("res/icono_recolectable.png", x, y, 20, 20, game) {

	animation = new Animation("res/recolectable.png", width, height,
		160, 20, 3, 8, true, game);
}

void PowerUpVida::update() {
	animation->update();
}

void PowerUpVida::draw(float scrollX) {
	animation->draw(x - scrollX, y);
}