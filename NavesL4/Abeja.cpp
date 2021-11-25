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
	bool endAnimation = animation->update();

	if (state == game->stateDying) {
		state = game->stateDead;
	}

	// Establecer velocidad
	if (state != game->stateDying) {
		// no está muerto y se ha quedado parado
		if (vy == 0) {
			vyIntelligence = vyIntelligence * -1;
			vy = vyIntelligence;
		}
		if (outRight) {
			// mover hacia la izquierda vx tiene que ser negativa
			if (vyIntelligence > 0) {
				vyIntelligence = vyIntelligence * -1;
			}
			vy = vyIntelligence;
		}
		if (outLeft) {
			// mover hacia la derecha vx tiene que ser positiva
			if (vyIntelligence < 0) {
				vyIntelligence = vyIntelligence * -1;
			}
			vy = vyIntelligence;
		}

	}
	else {
		vy = 0;
	}



}

void Abeja::impacted() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}


void Abeja::draw(float scrollX) {
	animation->draw(x - scrollX, y);
}