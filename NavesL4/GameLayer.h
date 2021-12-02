#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"

#include "Text.h"
#include "Tile.h"
#include "Pad.h"

#include "Audio.h"
#include "Space.h" // importar

#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include <list>
#include "Bomba.h"
#include "Abeja.h"
#include "Conejo.h"
#include "Moneda.h"

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event); // USO DE MOUSE
	void gamePadToControls(SDL_Event event); // USO DE GAMEPAD
	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	void calculateScroll();
	Actor* message;
	bool pause;
	// Elementos de interfaz
	SDL_GameController* gamePad;
	Pad* pad;
	Actor* buttonShoot;

	Space* space;
	float scrollX;
	int mapWidth;
	list<Tile*> tiles;

	Audio* audioBackground;

	Text* textVidas;
	Text* textEnemigos;
	int enemigosMatados;
	Text* textMonedas;
	int monedasRecogidas;

	Actor* backgroundVidas;
	Actor* backgroundEnemigos;
	Actor* backgroundMonedas;

	Player* player;
	Background* background;

	list<Abeja*> enemiesBees;
	list<Conejo*> enemiesRabbits;
	list<Tile*> bloquesLadrillo;
	list<Bomba*> bombas;
	list<Moneda*> monedas;

	bool controlContinue = false;
	bool controlPutBomb = false;
	int controlMoveY = 0;
	int controlMoveX = 0;
};

