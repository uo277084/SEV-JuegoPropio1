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

	space = new Space(1);
	scrollX = 0;
	scrollY = 0;
	tiles.clear();

	audioBackground = new Audio("res/music-background.mp3", true);
	audioBackground->play();

	points = 0;
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points);

	
	background = new Background("res/fondo.jpg", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);

	//Vaciar por si reiniciamos el juego
	enemiesRabbits.clear();
	enemiesRabbits.clear();
	bombas.clear();

	//loadMap("res/" + to_string(game->currentLevel) + ".txt");
	//loadMap("res/4.txt");
	loadMap("res/Level1.txt");
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
		printf("YAYAYAYYA");
		Conejo* conejo = new Conejo(x, y, game);
		conejo->y = conejo->y - conejo->height / 2;
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
	case 'M': {
		Tile* tile = new Tile("res/bloque_metal1.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
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
	//procesar controles
	//procesar controles
	// Disparar
	if (controlContinue) {
		pause = false;
		controlContinue = false;
	}
	/*
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			bombas.push_back(newProjectile);
		}

	}
	*/

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
	

	// Jugador se cae
	if (player->y > HEIGHT + 80) {
		init();
	}

	space->update();
	background->update();
	player->update();
	for (auto const& bee : enemiesBees) {
		bee->update();
	}
	for (auto const& rabbit : enemiesRabbits) {
		rabbit->update();
	}
	for (auto const& bomb : bombas) {
		bomb->update();
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
			if (player->lifes <= 0) {
				init();
				return;
			}
		}
	}

	// Colisiones , Enemy - Projectile

	list<Abeja*> deleteEnemiesBees;
	list<Conejo*> deleteEnemiesRabbits;
	list<Bomba*> deleteBombs;

	//TODO
	for (auto const& bomb : bombas) {
		if (bomb->isInRender(scrollX, scrollY) == false || bomb->vx == 0) {

			bool pInList = std::find(deleteBombs.begin(),
				deleteBombs.end(),
				bomb) != deleteBombs.end();

			if (!pInList) {
				deleteBombs.push_back(bomb);
			}
		}
	}

	for (auto const& bee : enemiesBees) {
		for (auto const& projectile : bombas) {
			if (bee->isOverlap(projectile)) {
				bool pInList = std::find(deleteBombs.begin(),
					deleteBombs.end(),
					projectile) != deleteBombs.end();

				if (!pInList) {
					deleteBombs.push_back(projectile);
				}

				bee->impacted();
				points++;
				textPoints->content = to_string(points);
			}
		}
	}

	for (auto const& rabbit : enemiesRabbits) {
		for (auto const& projectile : bombas) {
			if (rabbit->isOverlap(projectile)) {
				bool pInList = std::find(deleteBombs.begin(),
					deleteBombs.end(),
					projectile) != deleteBombs.end();

				if (!pInList) {
					deleteBombs.push_back(projectile);
				}

				rabbit->impacted();
				points++;
				textPoints->content = to_string(points);
			}
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
	if (player->y > HEIGHT * 0.5) {
		if (player->y - scrollY < HEIGHT * 0.5) {
			scrollY = player->y - HEIGHT * 0.5;
		}
	}

	if (player->y < mapWidth - HEIGHT * 0.5) {
		if (player->y - scrollY > HEIGHT * 0.5) {
			scrollY = player->y - HEIGHT * 0.5;
		}
	}
}


void GameLayer::draw() {
	calculateScroll();

	background->draw();
	for (auto const& tile : tiles) {
		tile->draw(scrollX, scrollY);
	}

	for (auto const& bomb : bombas) {
		bomb->draw(scrollX, scrollY);
	}
	player->draw(scrollX, scrollY);
	for (auto const& bee : enemiesBees) {
		bee->draw(scrollX, scrollY);
	}
	for (auto const& rabbit : enemiesRabbits) {
		rabbit->draw(scrollX, scrollY);
	}

	backgroundPoints->draw();
	textPoints->draw();

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
		controlShoot = true;
	}
	else {
		controlShoot = false;
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
			controlShoot = true;
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
			controlShoot = false;
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
			controlShoot = false;
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
			controlShoot = true;
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
			controlShoot = false;
			break;
		}

	}

}

