#pragma once

#include "Actor.h"
#include "Animation.h" 

class Conejo : public Actor
{
public:
	Conejo(float x, float y, Game* game);
	void draw(float scrollX = 0) override; // Va a sobrescribir
	void update();
	void impacted(); // Recibe impacto y pone animación de morir
	float vxIntelligence;
	int state;
	int lifes;
	Animation* aMovingDerecha;
	Animation* aMovingIzquierda;
	Animation* animation; // Referencia a la animación mostrada
};