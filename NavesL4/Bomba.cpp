#include "Bomba.h"

Bomba::Bomba(float x, float y, Game* game, bool powerUpAfectado) :
	Actor("res/bombaImage.png", x, y, 30, 30, game) {
	vx = 9;
	vy = -1; // La gravedad inicial es 1
	afectado = powerUpAfectado;
	state = stateEncendida;
	tiempoEncendida = 120;
	audioExplosion = new Audio("res/efecto_explosion.mp3", false);
	animacionBomba = new Animation("res/bomba.png", width, height, 90, 30, 6, 3, true, game);
	animacionExplosionPequeña = new Animation("res/explosion_pequeña.png", 90, 90, 360, 90, 8, 4, false, game);
	animacionExplosionGrande = new Animation("res/explosion_grande.png", 90, 90, 360, 90, 8, 4, false, game);
	animacion = animacionBomba;
}

void Bomba::update() {
	//vy = vy - 1; // La gravedad suma 1 en cada actualización restamos para anularla 
	bool endAnimation = animacion->update();
	if (endAnimation) {
		if (state == stateExplotando) {
			state = stateExplotada;
		}
	}
	if (tiempoEncendida == 0) {
		state = stateExplotando;
		tiempoEncendida = 120;
		width = height = 90;
		if (afectado) {
			// explosion pequeña
			animacion = animacionExplosionPequeña;
		}
		else {
			//explosion grande
			animacion = animacionExplosionGrande;
		}
	}
	else {
		tiempoEncendida--;
	}
}