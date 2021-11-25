#pragma once

#include "Actor.h"
#include "Animation.h" 

class Abeja : public Actor
{
public:
	Abeja(float x, float y, Game* game);
	void draw(float scrollX = 0) override; // Va a sobrescribir
	void update();
	void impacted(); // Recibe impacto y pone animación de morir
	float vyIntelligence;
	int state;
	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada
};