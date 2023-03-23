#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Ball {

	private:
		Vector2f m_Posn;
		RectangleShape m_Shape;

		float m_Speed = 500.0f;
		float m_DirY = .5f;
		float m_DirX = .5f;

	public:
		Ball( float startX, float startY);
		
		FloatRect getPosition();

		RectangleShape getShape();

		float getXVelocity();

		void reboundSides();

		void reboundPaddleOrTop();

		void reboundBottom();

		void update(Time dt);			

};
