#include "Tile.h"

Tile::Tile(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, 40, 32, game) {
	destructible = false;
}

Tile::Tile(string filename, float x, float y, int width, int height, Game* game) 
	: Actor(filename, x, y, width, height, game) {
	destructible = false;
}

Tile::Tile(string filename, float x, float y, int width, int height, Game* game, bool des) 
	: Actor(filename, x, y, width, height, game) {
	destructible = des;
}