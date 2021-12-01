#include "Conejo.h"

Conejo::Conejo(float x, float y, Game* game)
	: Actor("res/conejo.png", x, y, 30, 30, game) {
	
	state = game->stateMoving;

	lifes = 3;

	aMovingDerecha = new Animation("res/conejo-derecha.png", width, height,
		60, 30, 6, 2, true, game);
	aMovingIzquierda = new Animation("res/conejo-izquierda.png", width, height,
		60, 30, 6, 2, true, game);
	animation = aMovingDerecha;
	
	lifes = 3;
	vx = 1;
	vxIntelligence = -1;
	vx = vxIntelligence;
}

void Conejo::update() {
	// Actualizar la animaci�n
	animation->update();

	if (collisionRight) {
		//Se choca a la derecha
		animation = aMovingIzquierda;
		vx = vxIntelligence * 1;
	}
	else if (collisionLeft) {
		//Se choca a la izquierda
		animation = aMovingDerecha;
		vx = vxIntelligence * -1;
	}
}

void Conejo::impacted() {
	if (lifes == 1 && state != game->stateMoving) {
		state = game->stateDead;
	}
	else {
		lifes--;
	}
}

void Conejo::draw(float scrollX) {
	animation->draw(x - scrollX, y);
}