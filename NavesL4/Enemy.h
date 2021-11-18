#pragma once

#include "Actor.h"
#include "Animation.h" 

class Enemy : public Actor
{
public:
	Enemy(float x, float y, string filename, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	void update();
	void impacted(); // Recibe impacto y pone animaci�n de morir
	float vxIntelligence;
	int state;
	int lifes;
	Animation* aMoving;
	Animation* animation; // Referencia a la animaci�n mostrada
};