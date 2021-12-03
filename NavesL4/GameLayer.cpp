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

	buttonShoot = new Actor("res/boton_disparo.png", WIDTH * 0.75, HEIGHT * 0.83, 100, 100, game);

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

		// Cambio automático de input
		// PONER el GamePad
		if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERAXISMOTION) {
			game->input = game->inputGamePad;
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
			mouseToControls(event);
		}
		// Procesar Mando
		if (game->input == game->inputGamePad) {  // gamePAD
			gamePadToControls(event);
		}


	}
	// Poner bomba
	if (controlContinue) {
		pause = false;
		controlContinue = false;
	}

	if (controlPutBomb) {
		Bomba* bomba = player->putBomb();
		if (bomba != NULL) {
			bombas.push_back(bomba);
			space->addDynamicActor(bomba);
		}
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
	bool conditionEnemies = enemigosMatados == (enemiesBees.size() + enemiesRabbits.size());
	bool conditionCoins = monedasRecogidas == monedas.size();
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

	// Colisiones

	for (auto const& bee : enemiesBees) {
		if (player->isOverlap(bee)) {
			player->loseLife();
			if (player->lifes <= 0) {
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
		if (player->isOverlap(bomba) && bomba->state == bomba->stateExplotando) {
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

	//TODO

	for (auto const& bee : enemiesBees) {
		for (auto const& bomb : bombas) {
			if (bee->isOverlap(bomb) && bomb->state == bomb->stateExplotando) {
				bee->impacted();
				enemigosMatados++;
				textEnemigos->content = to_string(enemigosMatados) + "/" + to_string(numEnemigos);
			}
		}
	}

	for (auto const& rabbit : enemiesRabbits) {
		for (auto const& bomb : bombas) {
			if (rabbit->isOverlap(bomb) && bomb->state == bomb->stateExplotando) {
				rabbit->impacted();
				if (rabbit->state == game->stateDead) {
					enemigosMatados++;
					textEnemigos->content = to_string(enemigosMatados) + "/" + to_string(numEnemigos);
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

	textEnemigos->draw();
	textMonedas->draw();
	textVidas->draw();
	backgroundEnemigos->draw();
	backgroundMonedas->draw();
	backgroundVidas->draw();

	// HUD
	if (game->input == game->inputMouse) {
		buttonShoot->draw(); // NO TIENEN SCROLL, POSICION FIJA
		pad->draw(); // NO TIENEN SCROLL, POSICION FIJA
	}
	if (pause) {
		message->draw();
	}

	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::gamePadToControls(SDL_Event event) {

	// Leer los botones
	bool buttonA = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_A);
	bool buttonB = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_B);
	// SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B
	// SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_Y
	cout << "botones:" << buttonA << "," << buttonB << endl;
	int stickX = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_LEFTX);
	cout << "stickX" << stickX << endl;

	// Retorna aproximadamente entre [-32800, 32800], el centro debería estar en 0
	// Si el mando tiene "holgura" el centro varia [-4000 , 4000]
	if (stickX > 4000) {
		controlMoveX = 1;
	}
	else if (stickX < -4000) {
		controlMoveX = -1;
	}
	else {
		controlMoveX = 0;
	}

	if (buttonA) {
		controlPutBomb = true;
	}
	else {
		controlPutBomb = false;
	}

	if (buttonB) {
		controlMoveY = -1; // Saltar
	}
	else {
		controlMoveY = 0;
	}
}


void GameLayer::mouseToControls(SDL_Event event) {
	// Modificación de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;
	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		controlContinue = true;
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = true;
			// CLICK TAMBIEN TE MUEVE
			controlMoveX = pad->getOrientationX(motionX);
		}
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlPutBomb = true;
		}

	}
	// Cada vez que se mueve
	if (event.type == SDL_MOUSEMOTION) {
		if (pad->clicked && pad->containsPoint(motionX, motionY)) {
			controlMoveX = pad->getOrientationX(motionX);
			// Rango de -20 a 20 es igual que 0
			if (controlMoveX > -20 && controlMoveX < 20) {
				controlMoveX = 0;
			}

		}
		else {
			pad->clicked = false; // han sacado el ratón del pad
			controlMoveX = 0;
		}
		if (buttonShoot->containsPoint(motionX, motionY) == false) {
			controlPutBomb = false;
		}

	}
	// Cada vez que levantan el click
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = false;
			// LEVANTAR EL CLICK TAMBIEN TE PARA
			controlMoveX = 0;
		}

		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlPutBomb = false;
		}
	}
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
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
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
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
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

