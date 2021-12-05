#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	pause = true;
	message = new Actor("res/mensaje_como_jugar.png", WIDTH * 0.5, HEIGHT * 0.5,
		WIDTH, HEIGHT, game);

	gamePad = SDL_GameControllerOpen(0);
	init();
}


void GameLayer::init() {
	pad = new Pad(WIDTH * 0.15, HEIGHT * 0.80, game);

	space = new Space();
	scrollX = 0;
	tiles.clear();

	audioBackground = new Audio("res/music-background.mp3", true);
	audioBackground->play();

	enemigosMatados = 0;
	monedasRecogidas = 0;
	
	background = new Background("res/fondo.jpg", WIDTH * 0.5, HEIGHT * 0.5, -1, game);

	//Vaciar por si reiniciamos el juego
	enemiesBees.clear();
	enemiesRabbits.clear();
	bombas.clear();
	bloquesLadrillo.clear();
	monedas.clear();
	powerUpsMenosEfecto.clear();
	powerUpsMasVida.clear();

	loadMap("res/Level1.txt");
	//loadMap("res/Level2.txt");

	numEnemigos = enemiesBees.size() + enemiesRabbits.size();
	numMonedas = monedas.size();
	textVidas = new Text("hola", WIDTH * 0.09, HEIGHT * 0.04, game);
	textVidas->content = to_string(player->lifes);
	backgroundVidas = new Actor("res/cara-vidas.png",
		WIDTH * 0.04, HEIGHT * 0.04, 24, 24, game);
	textEnemigos = new Text("hola", WIDTH * 0.78, HEIGHT * 0.04, game);
	textEnemigos->content = to_string(enemigosMatados) + "/" + to_string(numEnemigos);
	backgroundEnemigos = new Actor("res/icono-enemigos.png",
		WIDTH * 0.71, HEIGHT * 0.04, 24, 24, game);
	textMonedas = new Text("hola", WIDTH * 0.94, HEIGHT * 0.04, game);
	textMonedas->content = to_string(monedasRecogidas) + "/" + to_string(numMonedas);
	backgroundMonedas = new Actor("res/icono-moneda.png",
		WIDTH * 0.865, HEIGHT * 0.04, 24, 24, game);
}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
			}

			cout << character << endl;
		}
	}
	streamFile.close();
}

void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
		case 'C': {
			Conejo* conejo = new Conejo(x, y, game);
			conejo->y = conejo->y - conejo->height /2;
			enemiesRabbits.push_back(conejo);
			space->addDynamicActor(conejo);
			break;
		}
		case 'A': {
			Abeja* abeja = new Abeja(x, y, game);
			abeja->y = abeja->y - abeja->height / 2;
			enemiesBees.push_back(abeja);
			space->addDynamicActor(abeja);
			break;
		}
		case '1': {
			player = new Player(x, y, game);
			// modificación para empezar a contar desde el suelo.
			player->y = player->y - player->height / 2;
			space->addDynamicActor(player);
			break;
		}
		case 'P': {
			Tile* tile = new Tile("res/bloque_metal.png", x, y, game);
			tile->y = tile->y - tile->height / 2;
			tiles.push_back(tile);
			space->addStaticActor(tile);
			break;
		}
		case 'M': {
			Moneda* moneda = new Moneda(x, y, game);
			moneda->y = moneda->y - 2 * (moneda->height / 3);
			monedas.push_back(moneda);
			space->addDynamicActor(moneda);
			break;
		}
		case 'U': {
			Tile* powerUp = new Tile("res/menos-efecto.jpg", x, y, 20, 20, game);
			powerUp->y = powerUp->y - 2 * (powerUp->height / 3);
			powerUpsMenosEfecto.push_back(powerUp);
			space->addDynamicActor(powerUp);
			break;
		}
		case 'V': {
			PowerUpVida* pv = new PowerUpVida(x, y, game);
			pv->y = pv->y - 2 * (pv->height / 3);
			powerUpsMasVida.push_back(pv);
			space->addDynamicActor(pv);
			break;
		}
		case 'L': {
			Tile* ladrillo = new Tile("res/bloque_ladrillo.png", x, y, 40, 32, game, true);
			ladrillo->y = ladrillo->y - ladrillo->height / 2;
			bloquesLadrillo.push_back(ladrillo);
			space->addStaticActor(ladrillo);
			break;
		}
	}
}


void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_CONTROLLERDEVICEADDED) {
			gamePad = SDL_GameControllerOpen(0);
			if (gamePad == NULL) {
				cout << "error en GamePad" << endl;
			}
			else {
				cout << "GamePad conectado" << endl;
			}
		}

		if (event.type == SDL_KEYDOWN) {
			game->input = game->inputKeyboard;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			game->input = game->inputMouse;
		}
		// Procesar teclas
		if (game->input == game->inputKeyboard) {
			keysToControls(event);
		}
		if (game->input == game->inputMouse) {
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				controlContinue = true;
			}
		}


	}
	// Poner bomba
	if (controlContinue) {
		pause = false;
		controlContinue = false;
	}

	if (controlPutBomb) {
		putBomb();
	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}
}

void GameLayer::update() {
	if (pause) {
		return;
	}
	// Nivel superado
	bool conditionEnemies = enemigosMatados == numEnemigos;
	bool conditionCoins = monedasRecogidas == numMonedas;
	if (conditionEnemies && conditionCoins) {
		game->currentLevel++;
		if (game->currentLevel > game->finalLevel) {
			game->currentLevel = 0;
		}
		message = new Actor("res/mensaje_ganar.png", WIDTH * 0.5, HEIGHT * 0.5,
			WIDTH, HEIGHT, game);
		pause = true;
		init();
	}

	space->update();
	player->update();

	for (auto const& bee : enemiesBees) {
		bee->update();
	}
	for (auto const& rabbit : enemiesRabbits) {
		rabbit->update();
	}
	for (auto const& bomba : bombas) {
		bomba->update();
	}
	for (auto const& pv : powerUpsMasVida) {
		pv->update();
	}

	// Colisiones

	for (auto const& bee : enemiesBees) {
		if (player->isOverlap(bee)) {
			player->loseLife();
			textVidas->content = to_string(player->lifes);
			if (player->lifes <= 0) {
				game->currentLevel++;
				if (game->currentLevel > game->finalLevel) {
					game->currentLevel = 0;
				}
				message = new Actor("res/mensaje_perder.png", WIDTH * 0.5, HEIGHT * 0.5,
					WIDTH, HEIGHT, game);
				pause = true;
				init();
				return;
			}
		}
	}
	for (auto const& rabbit : enemiesRabbits) {
		if (player->isOverlap(rabbit)) {
			player->loseLife();
			textVidas->content = to_string(player->lifes);
			if (player->lifes <= 0) {
				game->currentLevel++;
				if (game->currentLevel > game->finalLevel) {
					game->currentLevel = 0;
				}
				message = new Actor("res/mensaje_perder.png", WIDTH * 0.5, HEIGHT * 0.5,
					WIDTH, HEIGHT, game);
				pause = true;
				init();
				return;
			}
		}
	}

	for (auto const& bomba : bombas) {
		if (bomba->state == bomba->stateExplotando && isOverlapBomb(player, bomba)) {
			player->loseLife();
			textVidas->content = to_string(player->lifes);
			if (player->lifes <= 0) {
				game->currentLevel++;
				if (game->currentLevel > game->finalLevel) {
					game->currentLevel = 0;
				}
				message = new Actor("res/mensaje_perder.png", WIDTH * 0.5, HEIGHT * 0.5,
					WIDTH, HEIGHT, game);
				pause = true;
				init();
				return;
			}
		}
	}

	// Colisiones , Enemy - Projectile

	list<Abeja*> deleteEnemiesBees;
	list<Conejo*> deleteEnemiesRabbits;
	list<Bomba*> deleteBombs;
	list<Moneda*> deleteMonedas;
	list<Tile*> deletePowerUps;
	list<PowerUpVida*> deletePowerUpsVida;
	list<Tile*> deleteBloquesLadrillo;

	//TODO

	for (auto const& bee : enemiesBees) {
		for (auto const& bomb : bombas) {
			if (bomb->state == bomb->stateExplotando && isOverlapBomb(bee, bomb)) {
				bee->impacted();
				enemigosMatados++;
				textEnemigos->content = to_string(enemigosMatados) + "/" + to_string(numEnemigos);
			}
		}
	}

	for (auto const& rabbit : enemiesRabbits) {
		for (auto const& bomb : bombas) {
			if (bomb->state == bomb->stateExplotando && isOverlapBomb(rabbit, bomb)) {
				rabbit->impacted();
				if (rabbit->state == game->stateDead) {
					enemigosMatados++;
					textEnemigos->content = to_string(enemigosMatados) + "/" + to_string(numEnemigos);
				}
			}
		}
	}

	for (auto const& ladrillo : bloquesLadrillo) {
		for (auto const& bomb : bombas) {
			if (bomb->state == bomb->stateExplotando && isOverlapBombTile(ladrillo, bomb)) {
				bool eInList = std::find(deleteBloquesLadrillo.begin(),
					deleteBloquesLadrillo.end(),
					ladrillo) != deleteBloquesLadrillo.end();

				if (!eInList) {
					deleteBloquesLadrillo.push_back(ladrillo);
				}
			}
		}
	}

	for (auto const& moneda : monedas) {
		if (player->isOverlap(moneda)) {
			bool pInList = std::find(deleteMonedas.begin(),
				deleteMonedas.end(),
				moneda) != deleteMonedas.end();

			if (!pInList) {
				deleteMonedas.push_back(moneda);
			}
			monedasRecogidas++;
			textMonedas->content = to_string(monedasRecogidas) + "/" + to_string(numMonedas);
		}
	}

	for (auto const& powerUp : powerUpsMenosEfecto) {
		if (player->isOverlap(powerUp)) {
			bool pInList = std::find(deletePowerUps.begin(),
				deletePowerUps.end(),
				powerUp) != deletePowerUps.end();

			if (!pInList) {
				deletePowerUps.push_back(powerUp);
			}
			player->menosEfecto();
		}
	}

	for (auto const& powerUp : powerUpsMasVida) {
		if (player->isOverlap(powerUp)) {
			bool pInList = std::find(deletePowerUpsVida.begin(),
				deletePowerUpsVida.end(),
				powerUp) != deletePowerUpsVida.end();

			if (!pInList) {
				deletePowerUpsVida.push_back(powerUp);
			}
			player->addLife();
			textVidas->content = to_string(player->lifes);
		}
	}

	for (auto const& bee : enemiesBees) {
		if (bee->state == game->stateDead) {
			bool eInList = std::find(deleteEnemiesBees.begin(),
				deleteEnemiesBees.end(),
				bee) != deleteEnemiesBees.end();

			if (!eInList) {
				deleteEnemiesBees.push_back(bee);
			}
		}
	}

	for (auto const& rabbit : enemiesRabbits) {
		if (rabbit->state == game->stateDead) {
			bool eInList = std::find(deleteEnemiesRabbits.begin(),
				deleteEnemiesRabbits.end(),
				rabbit) != deleteEnemiesRabbits.end();

			if (!eInList) {
				deleteEnemiesRabbits.push_back(rabbit);
			}
		}
	}

	for (auto const& bomba : bombas) {
		if (bomba->state == bomba->stateExplotada) {
			bool eInList = std::find(deleteBombs.begin(),
				deleteBombs.end(),
				bomba) != deleteBombs.end();

			if (!eInList) {
				deleteBombs.push_back(bomba);
			}
		}
	}

	for (auto const& delEnemyBee : deleteEnemiesBees) {
		enemiesBees.remove(delEnemyBee);
		space->removeDynamicActor(delEnemyBee);
	}
	deleteEnemiesBees.clear();

	for (auto const& delEnemyRabbit : deleteEnemiesRabbits) {
		enemiesRabbits.remove(delEnemyRabbit);
		space->removeDynamicActor(delEnemyRabbit);
	}
	deleteEnemiesRabbits.clear();

	for (auto const& delBomb : deleteBombs) {
		bombas.remove(delBomb);
		space->removeDynamicActor(delBomb);
		delete delBomb;
	}
	deleteBombs.clear();

	for (auto const& delMoneda : deleteMonedas) {
		monedas.remove(delMoneda);
		space->removeDynamicActor(delMoneda);
		delete delMoneda;
	}
	deleteMonedas.clear();

	for (auto const& delPowerUp : deletePowerUps) {
		powerUpsMenosEfecto.remove(delPowerUp);
		space->removeDynamicActor(delPowerUp);
		delete delPowerUp;
	}
	deletePowerUps.clear();

	for (auto const& delPV : deletePowerUpsVida) {
		powerUpsMasVida.remove(delPV);
		space->removeDynamicActor(delPV);
		delete delPV;
	}
	deletePowerUpsVida.clear();

	for (auto const& delLadrillo : deleteBloquesLadrillo) {
		bloquesLadrillo.remove(delLadrillo);
		space->removeStaticActor(delLadrillo);
		delete delLadrillo;
	}
	deleteBloquesLadrillo.clear();

	cout << "update GameLayer" << endl;
}

void GameLayer::calculateScroll() {
	// limite izquierda
	if (player->x > WIDTH * 0.3) {
		if (player->x - scrollX < WIDTH * 0.3) {
			scrollX = player->x - WIDTH * 0.3;
		}
	}

	// limite derecha
	if (player->x < mapWidth - WIDTH * 0.3) {
		if (player->x - scrollX > WIDTH * 0.7) {
			scrollX = player->x - WIDTH * 0.7;
		}
	}
}

void GameLayer::draw() {
	calculateScroll();

	background->draw();

	for (auto const& tile : tiles) {
		tile->draw(scrollX);
	}
	for (auto const& tile : bloquesLadrillo) {
		tile->draw(scrollX);
	}
	
	for (auto const& bomb : bombas) {
		bomb->draw(scrollX);
	}
	player->draw(scrollX);
	for (auto const& bee : enemiesBees) {
		bee->draw(scrollX);
	}
	for (auto const& rabbit : enemiesRabbits) {
		rabbit->draw(scrollX);
	}
	for (auto const& moneda : monedas) {
		moneda->draw(scrollX);
	}
	for (auto const& powerUp : powerUpsMenosEfecto) {
		powerUp->draw(scrollX);
	}
	for (auto const& powerUpVida : powerUpsMasVida) {
		powerUpVida->draw(scrollX);
	}

	textEnemigos->draw();
	textMonedas->draw();
	textVidas->draw();
	backgroundEnemigos->draw();
	backgroundMonedas->draw();
	backgroundVidas->draw();

	if (pause) {
		message->draw();
	}

	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_RIGHT: // derecha
			controlMoveX = 1;
			break;
		case SDLK_LEFT: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_UP: // arriba
			controlMoveY = -1;
			break;
		case SDLK_DOWN: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlPutBomb = true;
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_RIGHT: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_LEFT: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_UP: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_DOWN: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlPutBomb = false;
			break;
		}
	}
}

void GameLayer::putBomb() {
	float xb = calculateXBomb();
	float yb = calculateYBomb();

	string direction = bombDirection(xb, yb);

	Bomba* bomba = player->putBomb(xb, yb, direction);
	if (bomba != NULL) {
		bombas.push_back(bomba);
		space->addDynamicActor(bomba);
	}
}

string GameLayer::bombDirection(float xb, float yb) {
	//Mirar a ver si hay tiles o tiles destructibles alrededor
	string direction = "";
	//Tile izquierda
	Actor* tile = space->hayTile(xb-40, yb);
	Tile* t;
	if (tile != NULL) {
		t = (Tile*)tile;
		if (t->destructible) {
			direction += "l";
		}
	}
	else {
		direction += "l";
	}
	//Tile derecha
	tile = space->hayTile(xb + 40, yb);
	if (tile != NULL) {
		t = (Tile*)tile;
		if (t->destructible) {
			direction += "r";
		}
	}
	else {
		direction += "r";
	}
	//Tile arriba
	tile = space->hayTile(xb, yb - 32);
	if (tile != NULL) {
		t = (Tile*)tile;
		if (t->destructible) {
			direction += "u";
		}
	}
	else {
		direction += "u";
	}
	//Tile abajo
	tile = space->hayTile(xb, yb + 32);
	if (tile != NULL) {
		t = (Tile*)tile;
		if (t->destructible) {
			direction += "d";
		}
	}
	else {
		direction += "d";
	}
	return direction;
}

float GameLayer::calculateXBomb() {
	float inicio = 0;
	float xb = 20;
	float final = 40;
	while (!(player->x >= inicio && player->x < final)) {
		//Mientras no este entre el inicio y el final
		inicio = final;
		final += 40;
		xb = (final + inicio) / 2;
	}
	return xb;
}

float GameLayer::calculateYBomb() {
	float inicio = 0;
	float yb = 16;
	float final = 32;
	while (!(player->y >= inicio && player->y < final)) {
		//Mientras no este entre el inicio y el final
		inicio = final;
		final += 32;
		yb = (final + inicio) / 2;
	}
	return yb;
}

bool GameLayer::isOverlapBomb(Actor* actor, Bomba* bomb)
{
	float xa = actor->x;
	float ya = actor->y;
	float xb = bomb->x;
	float yb = bomb->y;

	if (!bomb->afectado) {
		if (bomb->izq) {
			if (xb - (bomb->width / 2) <= xa + (actor->width / 2) && xa <= xb && ya <= yb + 20 && yb - 20 <= ya) {
				return true;
			}
		}
		if (bomb->der) {
			if (xb <= xa && xa - (actor->width / 2) <= xb + (bomb->width / 2) && ya <= yb + 20 && yb - 20 <= ya) {
				return true;
			}
		}
		if (bomb->up) {
			if (yb - (bomb->height / 2) <= ya + (actor->height / 2) && ya <= yb && xa <= xb + 20 && xb - 20 <= xa) {
				return true;
			}
		}
		if (bomb->down) {
			if (yb <= ya && ya - (actor->height / 2) <= yb + (bomb->height / 2) && xa <= xb + 20 && xb - 20 <= xa) {
				return true;
			}
		}
		return false;
	}
	else {
		return actor->isOverlap(bomb);
	}
}

bool GameLayer::isOverlapBombTile(Tile* tile, Bomba* bomb)
{
	float xa = tile->x;
	float ya = tile->y;
	float xb = bomb->x;
	float yb = bomb->y;
	float na, nb;

	if (!bomb->afectado) {
		if (bomb->izq) {
			na = (xa + tile->width / 2);	//parte der del tile
			nb = (xb - bomb->width / 2);	//parte izq de la bomba
			if (ya == yb && na >= nb) {
				return true;
			}
		}
		if (bomb->der) {
			na = (xa - bomb->width / 2);	//parte izq del tile
			nb = (xb + bomb->width / 2);	//parte der de la bomba
			if (ya == yb && nb >= na) {
				return true;
			}
		}
		if (bomb->up) {
			na = (ya + tile->height / 2);	//parte abajo del tile
			nb = (yb - bomb->height / 2);	//parte arriba de la bomba
			if (xa == xb && na >= nb) {
				return true;
			}
		}
		if (bomb->down) {
			na = (ya - tile->height);	//parte arriba del tile
			nb = (yb + bomb->height / 2);	//parte abajo de la bomba
			if (xa == xb && nb >= na) {
				return true;
			}
		}
		return false;
	}
	else {
		return tile->isOverlap(bomb);
	}
}
