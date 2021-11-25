#include "Conejo.h"

Conejo::Conejo(float x, float y, Game* game)
	: Actor("res/conejo.png", x, y, 35, 35, game) {

	state = game->stateMoving;

	lifes = 3;

	aMovingDerecha = new Animation("res/conejo-derecha.png", width, height,
		70, 35, 6, 2, true, game);
	aMovingIzquierda = new Animation("res/conejo-izquierda.png", width, height,
		70, 35, 6, 2, true, game);
	animation = aMovingDerecha;

	vx = 1;
	vxIntelligence = -1;
	vx = vxIntelligence;

}

void Conejo::update() {
	// Actualizar la animación
	bool endAnimation = animation->update();

	if (state == game->stateDying) {
		state = game->stateDead;
	}

	// Establecer velocidad
	if (state != game->stateDying) {
		// no está muerto y se ha quedado parado
		if (vx == 0) {
			vxIntelligence = vxIntelligence * -1;
			vx = vxIntelligence;
		}
		if (outRight) {
			// mover hacia la izquierda vx tiene que ser negativa
			if (vxIntelligence > 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
		}
		if (outLeft) {
			// mover hacia la derecha vx tiene que ser positiva
			if (vxIntelligence < 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
		}

	}
	else {
		vx = 0;
	}



}

void Conejo::impacted() {
	if (lifes == 1 &&state != game->stateDying) {
		state = game->stateDying;
	}
	else {
		lifes--;
	}
}

void Conejo::draw(float scrollX) {
	animation->draw(x - scrollX, y);
}