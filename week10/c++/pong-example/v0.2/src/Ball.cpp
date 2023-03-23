#include "Ball.h"

// constructor
Ball::Ball(float startX, float startY) {

	m_Posn.x = startX;
	m_Posn.y = startY;

	m_Shape.setSize(sf::Vector2f(10, 10));
	m_Shape.setPosition(m_Posn);

}

/*
* Getter FNs
*/
FloatRect Ball::getPosition() {
	return m_Shape.getGlobalBounds();
}

RectangleShape Ball::getShape() {
	return m_Shape;
}

float Ball::getXVelocity() {
	return m_DirX;
}

/*
* Rebound FNs
* - handlers for ball after defined collision
*/
void Ball::reboundSides() {
	m_DirX = -m_DirX;
}

void Ball::reboundPaddleOrTop() {
	m_DirY = -m_DirY;
}

void Ball::reboundBottom() {
	m_Posn.y = 20;
	m_Posn.x = 500;
//	m_DirY = -m_DirY;
}

/*
* Update FN
*/
void Ball::update(Time dt) {
	// update ball's posn
	m_Posn.y += m_DirY * m_Speed * dt.asSeconds();
	m_Posn.x += m_DirX * m_Speed * dt.asSeconds();

	// update ball motion
	m_Shape.setPosition(m_Posn);
}
