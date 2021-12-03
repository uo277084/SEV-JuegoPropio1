#pragma once
#include "Actor.h"
#include "Animation.h"

class PowerUpVida : public Actor
{
public:
	PowerUpVida(float x, float y, Game* game);
	Animation* animation;
	void draw(float scrollX = 0) override; // Va a sobrescribir
	void update();
};