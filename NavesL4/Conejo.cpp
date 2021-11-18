#include "Conejo.h"

Conejo::Conejo(float x, float y, Game* game)
	: Actor("res/conejo.png", x, y, 35, 35, game) {

	state = game->stateMoving;

	lifes = 3;

	aMoving = new Animation("res/enemigo_movimiento.png", width, height,
		32, 16, 6, 2, true, game);
	animation = aMoving;

	vx = 1;
	vxIntelligence = -1;
	vx = vxIntelligence;

}

void Conejo::update() {
	// Actualizar la animaci�n
	bool endAnimation = animation->update();

	if (state == game->stateDying) {
		state = game->stateDead;
	}

	// Establecer velocidad
	if (state != game->stateDying) {
		// no est� muerto y se ha quedado parado
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

void Conejo::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}