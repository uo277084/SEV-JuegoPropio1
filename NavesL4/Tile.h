#pragma once
#include "Actor.h"

class Tile : public Actor
{
public:
	Tile(string filename, float x, float y, Game* game);
	Tile(string filename, float x, float y, int width, int height, Game* game);
	Tile(string filename, float x, float y, int width, int height, Game* game, bool des);
	bool destructible;
};

