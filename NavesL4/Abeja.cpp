#include "Abeja.h"

Abeja::Abeja(float x, float y, Game* game)
	: Actor("res/abeja.png", x, y, 25, 25, game) {

	state = game->stateMoving;

	aMoving = new Animation("res/abeja-mov.png", width, height,
		32, 16, 6, 2, true, game);
	animation = aMoving;

	vy = 1;
	vyIntelligence = -1;
	vy = vyIntelligence;

}

void Abeja::update() {
	// Actualizar la animación
	animation->update();

	if (collisionDown) {
		//Se choca abajo
		vy = vyIntelligence * 1;
	}
	else if (collisionUp) {
		//Se choca arriba
		vy = vyIntelligence * -1;
	}
}

void Abeja::impacted() {
	if (state != game->stateDead) {
		state = game->stateDead;
	}
}


void Abeja::draw(float scrollX) {
	animation->draw(x - scrollX, y);
}