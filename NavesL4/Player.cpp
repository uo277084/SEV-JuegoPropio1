#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/bomberman.png", x, y, 35, 35, game) {

	orientation = game->orientationRight;
	state = game->stateMoving;
	audioBomba = new Audio("res/efecto_disparo.wav", false);
	aBombaRight = new Animation("res/jugador_disparando_derecha.png",
		width, height, 160, 40, 6, 4, false, game);
	aBombaLeft = new Animation("res/jugador_disparando_izquierda.png",
		width, height, 160, 40, 6, 4, false, game);

	aIdleRight = new Animation("res/jugador-parado-derecha.png", width, height,
		35, 35, 6, 1, true, game);
	aIdleLeft = new Animation("res/jugador-parado-izquierda.png", width, height,
		35, 35, 6, 1, true, game);
	aRunningRight = new Animation("res/jugador-caminando-derecha.png", width, height,
		140, 35, 6, 4, true, game);
	aRunningLeft = new Animation("res/jugador-caminando-izquierda.png", width, height,
		140, 35, 6, 4, true, game);

	animation = aIdleRight;

}


void Player::update() {
	if (invulnerableTime > 0) {
		invulnerableTime--;
	}

	bool endAnimation = animation->update();

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba disparando
		if (state == game->stateShooting) {
			state = game->stateMoving;
		}
	}


	// Establecer orientación
	if (vx > 0) {
		orientation = game->orientationRight;
	}
	if (vx < 0) {
		orientation = game->orientationLeft;
	}


	// Selección de animación basada en estados
	if (state == game->stateShooting) {
		if (orientation == game->orientationRight) {
			animation = aBombaRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aBombaLeft;
		}
	}
	if (state == game->stateMoving) {
		if (vx != 0) {
			if (orientation == game->orientationRight) {
				animation = aRunningRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aRunningLeft;
			}
		}
		if (vx == 0) {
			if (orientation == game->orientationRight) {
				animation = aIdleRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aIdleLeft;
			}
		}
	}


	if (shootTime > 0) {
		shootTime--;
	}

}

void Player::moveX(float axis) {
	vx = axis * 3;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}

Bomba* Player::shoot() {

	if (shootTime == 0) {
		state = game->stateShooting;
		audioBomba->play();
		aBombaLeft->currentFrame = 0; //"Rebobinar" aniamción
		aBombaRight->currentFrame = 0; //"Rebobinar" aniamción
		shootTime = shootCadence;
		Bomba* projectile = new Bomba(x, y, game);
		if (orientation == game->orientationLeft) {
			projectile->vx = projectile->vx * -1; // Invertir
		}
		return projectile;
	}
	else {
		return NULL;
	}
}

void Player::draw(float scrollX, float scrollY) {
	if (invulnerableTime == 0) {
		animation->draw(x - scrollX, y - scrollY);
	}
	else {
		if (invulnerableTime % 10 >= 0 && invulnerableTime % 10 <= 5) {
			animation->draw(x - scrollX, y - scrollY);
		}
	}
}

void Player::loseLife() {
	if (invulnerableTime <= 0) {
		if (lifes > 0) {
			lifes--;
			invulnerableTime = 100;
			// 100 actualizaciones 
		}
	}
}
