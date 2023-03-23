#include "Paddle.h"

// define constructor for the paddle
Paddle::Paddle(float startX, float startY) {
	m_Posn.x = startX;
	m_Posn.y = startY;

	m_RectShape.setSize((sf::Vector2f(50, 5)));
	m_RectShape.setPosition(m_Posn);
}

FloatRect Paddle::getPosition() {
	return m_RectShape.getGlobalBounds();
}

RectangleShape Paddle::getShape() {
	return m_RectShape;
}

void Paddle::motionLeft() {
	m_MotionLeft = true;
}

void Paddle::motionRight() {
	m_MotionRight = true;
}

void Paddle::stopLeft() {
	m_MotionLeft = false;
}

void Paddle::stopRight() {
	m_MotionRight = false;
}

void Paddle::update(Time dt) {
	if (m_MotionLeft) {
		m_Posn.x -= m_Speed * dt.asSeconds();
	} 

	if (m_MotionRight) {
		m_Posn.x += m_Speed * dt.asSeconds();
	} 
	m_RectShape.setPosition(m_Posn);
}



