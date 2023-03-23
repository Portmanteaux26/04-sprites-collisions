#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Paddle {

	private:
		Vector2f m_Posn;

		// rectangle shape
		RectangleShape m_RectShape;

		float m_Speed = 1000.0f;

		bool m_MotionRight = false;
		bool m_MotionLeft = false;

	public:
		Paddle(float startX, float startY);

		FloatRect getPosition();

		RectangleShape getShape();

		void motionLeft();
		void motionRight();
		void stopLeft();
		void stopRight();

		void update(Time dt);

};
