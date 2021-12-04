#include "Bomba.h"

Bomba::Bomba(float x, float y, Game* game, bool powerUpAfectado, string direction) :
	Actor("res/bombaImage.png", x, y, 20, 30, game) { 
	afectado = powerUpAfectado;
 	state = stateEncendida;
	tiempoEncendida = 100;

	calculateDirection(direction);

	audioExplosion = new Audio("res/efecto_explosion.wav", false);
	animacionBomba = new Animation("res/bomba.png", width, height, 60, 30, 4, 3, true, game);
	animacionExplosionLittle = new Animation("res/explosion_little.png", 45, 45, 180, 45, 3, 4, false, game);
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
			// explosion pequeña
			width = height = 45;
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
	string left = "l";
	string right = "r";
	string up = "u";
	string down = "d";
	//str.find(str2) != string::npos
	if (direction.find(left) != std::string::npos) {
		izq = true;
	}
	if (direction.find(right) != std::string::npos) {
		der = true;
	}
	if (direction.find(up) != std::string::npos) {
		up = true;
	}
	if (direction.find(down) != std::string::npos) {
		down = true;
	}
}