#include "Tile.h"

Tile::Tile(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, 40, 32, game) {
}

Tile::Tile(string filename, float x, float y, int width, int height, Game* game) 
	: Actor(filename, x, y, width, height, game) {
}