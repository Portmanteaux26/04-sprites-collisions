#include <cmath>
#include "Player.h"

// constructor for player object
// - instantiate player once per game lifecycle
Player::Player() {
	// define initial values
	m_Speed = initial_Speed;
	m_HealthStatus = initial_Health;
	m_HealthMax = initial_Health;

	// setup texture for player's sprite
	m_Texture.loadFromFile("../assets/graphics/player.png");
	m_Sprite.setTexture(m_Texture);

	// origin of sprite set to centre
	m_Sprite.setOrigin(25, 25);
}

// spawn function for loading player
// may be called multiple times in game lifecycle
void Player::spawn(IntRect arena, Vector2f screenRes, int tileSize) {
	// set initial player posn to centre of arena
	m_Posn.x = arena.width / 2;
	m_Posn.y = arena.height / 2;

	// arena properties copied to player's member obj
	m_Arena.left = arena.left;
	m_Arena.top = arena.top;
	m_Arena.width = arena.width;
	m_Arena.height = arena.height;

	// tile properties copied to player's member obj
	m_TileSize = tileSize;

	// screen res properties copied to player's member obj
	m_ScreenRes.x = screenRes.x;
	m_ScreenRes.y = screenRes.y;		
}

void Player::resetPlayerStats() {
	m_Speed = initial_Speed;
	m_HealthStatus = initial_Health;
	m_HealthMax = initial_Health;
}

Time Player::getTimeLastHit() {
	return m_LastHit;
}

bool Player::hit(Time timeHit) {
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200) {
		m_LastHit = timeHit;
		m_HealthStatus -= 10;
		return true;
	} else {
		return false;
	}
}	

/*
* getters
*/

FloatRect Player::getPlayerPosn() {
	return m_Sprite.getGlobalBounds();
}

Vector2f Player::getPlayerCentre() {
	return m_Posn;
}

float Player::getPlayerRotation() {
	return m_Sprite.getRotation();
}

Sprite Player::getPlayerSprite() {
	return m_Sprite;
}

int Player::getPlayerHealth() {
	return m_HealthStatus;
}

/*
* motion handlers
*/

void Player::motionUp() {
	m_UpActive = true;
}
void Player::motionDown() {
	m_DownActive = true;
}
void Player::motionLeft() {
	m_LeftActive = true;
}
void Player::motionRight() {
	m_RightActive = true;
}

void Player::stopUp() {
	m_UpActive = false;
}
void Player::stopDown() {
	m_DownActive = false;
}
void Player::stopLeft() {
	m_LeftActive = false;
}
void Player::stopRight() {
	m_RightActive = false;
}

/*
* UPDATE
* - called once per frame in game loop
*/
void Player::update(float timeElapsed, Vector2i mousePosn) {
	if (m_UpActive) {
		m_Posn.y -= m_Speed * timeElapsed;
	}	

	if (m_DownActive) {
		m_Posn.y += m_Speed * timeElapsed;
	}

	if (m_RightActive) {
		m_Posn.x += m_Speed * timeElapsed;
	}

	if (m_LeftActive) {
		m_Posn.x -= m_Speed * timeElapsed;
	}

	m_Sprite.setPosition(m_Posn);

	// check and keep player in arena space
	if (m_Posn.x > m_Arena.width - m_TileSize) {
		m_Posn.x = m_Arena.width - m_TileSize;
	}

	if (m_Posn.x < m_Arena.left + m_TileSize) {
		m_Posn.x = m_Arena.left + m_TileSize;
	}

	if (m_Posn.y > m_Arena.height - m_TileSize) {
		m_Posn.y = m_Arena.height - m_TileSize;
	}

	if (m_Posn.y < m_Arena.top + m_TileSize) {
		m_Posn.y = m_Arena.top + m_TileSize;
	}

	// calculate angle of rotation for player
	float angle = (atan2(mousePosn.y - m_ScreenRes.y / 2, mousePosn.x - m_ScreenRes.x / 2) * 180) / 3.141;

	m_Sprite.setRotation(angle);
}		

/*
* BONUS
* - update player's health, speed &c.
* - use bonus options
*/

void Player::bonusSpeed() {
	// bonus increade by 20%
	m_Speed += (initial_Speed * .2);
}
void Player::bonusHealth() {
	// bonus increase by 20%
	m_HealthMax += (initial_Health * .2);
	}
void Player::incrHealthMax(int amount) {
	m_HealthStatus += amount;

	// check health level against max - do not exceed
	if(m_HealthStatus > m_HealthMax) {
		m_HealthStatus = m_HealthMax;
	}

}



