#include "Bomba.h"

Bomba::Bomba(float x, float y, Game* game, bool powerUpAfectado, string direction) :
	Actor("res/bombaImage.png", x, y, 20, 30, game) { 
	afectado = powerUpAfectado;
 	state = stateEncendida;
	tiempoEncendida = 100;

	calculateDirection(direction);

	audioExplosion = new Audio("res/efecto_explosion.wav", false);

	animacionBomba = new Animation("res/bomba.png", width, height, 60, 30, 4, 3, true, game);

	animacionExplosionLittle = new Animation("res/explosion_little.png", 30, 30, 120, 30, 3, 4, false, game);
	animacionExplosionGrande = new Animation("res/explosion_big_" + direction + ".png", 90, 90, 360, 90, 3, 4, false, game);
	
	animacion = animacionBomba;
}

void Bomba::update() {
	bool endAnimation = animacion->update();

	if (endAnimation) {
		if (state == stateExplotando) {
			state = stateExplotada;
		}
	}

	if (tiempoEncendida == 0) {
		if (afectado) {
			// explosion peque?a
			width = height = 30;
			animacion = animacionExplosionLittle;
		}
		else {
			//explosion grande
			width = height = 90;
 			animacion = animacionExplosionGrande;
		}
		audioExplosion->play();
		state = stateExplotando; 
		tiempoEncendida = 100;
	}
	else {
		tiempoEncendida--;
	}
}

void Bomba::draw(float scrollX) {
	animacion->draw(x - scrollX, y);
}

void Bomba::calculateDirection(string direction) {
	string l = "l";
	string r = "r";
	string u = "u";
	string d = "d";

	if (direction.find(l) != std::string::npos) {
		izq = true;
	}
	if (direction.find(r) != std::string::npos) {
		der = true;
	}
	if (direction.find(u) != std::string::npos) {
		up = true;
	}
	if (direction.find(d) != std::string::npos) {
		down = true;
	}
}