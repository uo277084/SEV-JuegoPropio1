#pragma once

#include "Actor.h"
#include "Bomba.h" 
#include "Audio.h"
#include "Animation.h" // incluir animacion 

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	Bomba* shoot();
	void update();
	void moveX(float axis);
	void moveY(float axis);
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	void loseLife();
	int lifes = 3;
	int invulnerableTime = 0;
	int orientation;
	int state;
	Animation* aIdleRight;
	Animation* aIdleLeft;
	Animation* aIdleUp;
	Animation* aIdleDown;
	Animation* aRunningRight;
	Animation* aRunningLeft;
	Animation* aRunningUp;
	Animation* aRunningDown;
	Animation* aBombaRight;
	Animation* aBombaLeft;
	Animation* aBombaUp;
	Animation* aBombaDown;
	Animation* animation; // Referencia a la animación mostrada
	Audio* audioBomba;
	int shootCadence = 30;
	int shootTime = 0;
};

