#pragma once

#include "Actor.h"
#include "Bomba.h" 
#include "Audio.h"
#include "Animation.h" // incluir animacion 

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	void update();
	void moveX(float axis);
	void moveY(float axis);
	void draw(float scrollX = 0) override; // Va a sobrescribir
	void loseLife();
	void addLife();
	Bomba* putBomb(float x, float y);
	void menosEfecto();
	int lifes = 3;
	int invulnerableTime = 0;
	int orientation;
	int state;
	bool anim;
	Animation* aRunningRight;
	Animation* aRunningLeft;
	Animation* aRunningUp;
	Animation* aRunningDown;
	Animation* aIdleRight;
	Animation* aIdleLeft;
	Animation* aIdleUp;
	Animation* aIdleDown;
	Animation* animation; // Referencia a la animación mostrada
	Audio* audioBomba;
	int shootCadence = 50;
	int shootTime = 0;
	bool afectado;
	int timeAfectado;
	int afectadoCadence = 200;
	int afectadoTime = 0;
};

