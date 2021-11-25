#pragma once

#include "Actor.h"
#include "Audio.h"
#include "Animation.h"

class Bomba : public Actor
{
public:
	Bomba(float x, float y, Game* game, bool afectado);
	void update();
	Audio* audioExplosion;
	Animation* animacionBomba;
	Animation* animacionExplosionGrande;
	Animation* animacionExplosionPequeña;
	Animation* animacion;
	bool afectado;
	int tiempoEncendida;
	int stateEncendida = 0;
	int stateExplotando = 1;
	int stateExplotada = 2;
	int state;
};