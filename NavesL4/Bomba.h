#pragma once

#include "Actor.h"


class Bomba : public Actor
{
public:
	Bomba(float x, float y, Game* game);
	void update();
};

