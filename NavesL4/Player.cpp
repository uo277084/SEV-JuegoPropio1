#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador-parado-down.png", x, y, 30, 30, game) {

	orientation = game->orientationDown;
	state = game->stateMoving;
	audioBomba = new Audio("res/efecto_disparo.wav", false);

	aRunningRight = new Animation("res/jugador-caminando-derecha.png", width, height,
		140, 35, 6, 4, true, game);
	aRunningLeft = new Animation("res/jugador-caminando-izquierda.png", width, height,
		140, 35, 6, 4, true, game);
	aRunningUp = new Animation("res/jugador-caminando-arriba.png", width, height,
		140, 35, 6, 4, true, game);
	aRunningDown = new Animation("res/jugador-caminando-abajo.png", width, height,
		140, 35, 6, 4, true, game); 
	aIdleRight = new Animation("res/jugador-parado-derecha.png", width, height,
		35, 35, 6, 1, true, game);
	aIdleLeft = new Animation("res/jugador-parado-izquierda.png", width, height,
		35, 35, 6, 1, true, game);
	aIdleUp = new Animation("res/jugador-parado-up.png", width, height,
		35, 35, 6, 1, true, game);
	aIdleDown = new Animation("res/jugador-parado-down.png", width, height,
		35, 35, 6, 1, true, game);

	animation = aIdleDown;
	orientation = game->orientationDown;
}


void Player::update() {
	if (invulnerableTime > 0) {
		invulnerableTime--;
	}

	if (timeAfectado == 0) {
		timeAfectado = 300;
		afectado = false;
	}

	// Establecer orientación
	if (vx > 0 && vy == 0) {
		orientation = game->orientationRight;
	}
	if (vx < 0 && vy == 0) {
		orientation = game->orientationLeft;
	}
	if (vx == 0 && vy > 0) {
		orientation = game->orientationDown;
	}
	if (vx == 0 && vy < 0) {
		orientation = game->orientationUp;
	}

	if (state == game->stateMoving) {
		if (vx != 0 && vy == 0) {
			if (orientation == game->orientationRight) {
				animation = aRunningRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aRunningLeft;
			}
		}
		if (vx == 0 && vy != 0) {
			if (orientation == game->orientationUp) {
				animation = aRunningUp;
			}
			if (orientation == game->orientationDown) {
				animation = aRunningDown;
			}
		}
		if (vx == 0 && vy == 0) {
			if(orientation == game->orientationRight) {
				animation = aIdleRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aIdleLeft;
			}
			if (orientation == game->orientationUp) {
				animation = aIdleUp;
			}
			if (orientation == game->orientationDown) {
				animation = aIdleDown;
			}
		}
	}
	if (shootTime > 0) {
		shootTime--;
	}
	if (afectadoTime > 0) {
		afectadoTime--;
	}
	else if (afectadoTime == 0) {
		afectado = false;
	}
	animation->update();
}

void Player::moveX(float axis) {
	vx = axis * 3;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}

void Player::draw(float scrollX) {
	if (invulnerableTime == 0) {
		animation->draw(x - scrollX, y);
	}
	else {
		if (invulnerableTime % 10 >= 0 && invulnerableTime % 10 <= 5) {
			animation->draw(x - scrollX, y);
		}
	}
}

void Player::loseLife() {
	if (invulnerableTime <= 0) {
		if (lifes > 0) {
			lifes--;
			invulnerableTime = 100;
		}
	}
}

Bomba* Player::putBomb(float xb, float yb)
{
	if (shootTime == 0) {
		shootTime = shootCadence;
		Bomba* bomba = new Bomba(xb, yb, game, afectado);
		return bomba;
	}
	else {
		return NULL;
	}
}

void Player::menosEfecto()
{
	afectadoTime = afectadoCadence;
	afectado = true;
}

void Player::addLife() {
	lifes++;
}