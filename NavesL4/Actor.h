#pragma once

#include "Game.h"

class Actor
{
public:
	Actor(string filename, float x, float y, int width, int height, Game* game);
	~Actor();
	virtual void draw(float scrollX = 0);
	bool isOverlap(Actor* actor);
	bool containsPoint(int pointX, int pointY); // contiene punto
	bool isInRender(float scrollX = 0, float scrollY = 0);
	bool collisionDown;
	bool collisionUp;
	bool collisionRight;
	bool collisionLeft;

	bool clicked; // Est� pulsado
	SDL_Texture* texture;
	int x;
	int y;
	float vx;
	float vy;
	int width;
	int height;
	int fileWidth;
	int fileHeight;
	Game* game; // referencia al juego
};

