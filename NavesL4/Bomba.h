#pragma once

#include "Actor.h"
#include "Audio.h"
#include "Animation.h"

class Bomba : public Actor
{
public:
	Bomba(float x, float y, Game* game, bool afectado, string direction);
	void update();
	void draw(float scrollX = 0) override; // Va a sobrescribir
	void calculateDirection(string direction);
	Audio* audioExplosion;
	Animation* animacionBomba;
	Animation* animacionExplosionGrande;
	Animation* animacionExplosionLittle;
	Animation* animacion;
	bool afectado;
	bool izq;
	bool der;
	bool up;
	bool down;
	int tiempoEncendida;
	int stateEncendida = 0;
	int stateExplotando = 1;
	int stateExplotada = 2;
	int state;
};