#include "Ball.h"
#include "Paddle.h"
#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>

// initial bootstrap for game
int main() {

	// add video mode obj - full HD window
	VideoMode vm(1920, 1080);

	// open window for game
	RenderWindow window(vm, "PongSpire", Style::Fullscreen);

	// initial game settings
	int score = 0;
	int lives = 5;

	// add initial ball
	Ball ball(1920 / 2, 10);
	// add initial paddle - place at foot of window, centred
	Paddle paddle(1920 / 2, 1080 - 20);	

	// add text object for heads-up display
	Text hud;

	// add font for display
	Font font;
	font.loadFromFile("../assets/fonts/Vera.ttf");

	// set styles for display
	hud.setFont(font);
	hud.setCharacterSize(30);
	hud.setFillColor(Color::White);
	hud.setPosition(20, 20);

	// define clock for game timing
	Clock clock;
	
	// initial game loop
	while (window.isOpen()) {

		/*
		* handle player input
		*/
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				// close window & quit game
				window.close();
			}
		}
		// handle player exiting game
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		// handle keypress for left arrow
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			paddle.motionLeft();
		} else {
			paddle.stopLeft();
		}

		// handle keypress for right arrow
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			paddle.motionRight();	
		}	else {
			paddle.stopRight();
		}

		/*
		* updates
		* - paddle, ball, display
		*/
		// update delta time
		Time dt = clock.restart();
		paddle.update(dt);
		ball.update(dt);
		// update display
		std::stringstream ss;
		ss << "Score:" << score << " Lives:" << lives;
		hud.setString(ss.str());
		
		/*
		* check for collisions
		*/
		// ball hiting bottom of game window
		if (ball.getPosition().top > window.getSize().y) {
			// change ball direction - reverse
			ball.reboundBottom();
			// update game values - player lives
			lives--;
			// check for no lives remaining
			if (lives < 1) {
				// reset game values - score & lives
				score = 0;
				lives = 5;
			}
		}
		// ball hitting top of game window
		if (ball.getPosition().top < -1) {
			ball.reboundPaddleOrTop();
			// update game values - score
			score++;
		}
		// ball hitting sides
		if (ball.getPosition().left < 0 || ball.getPosition().left + ball.getPosition().width > window.getSize().x) {
			ball.reboundSides();
		}
		// ball hits paddle
		if (ball.getPosition().intersects(paddle.getPosition())) {
			// collision detected between paddle and ball
			ball.reboundPaddleOrTop();
		}

		/*
		* draw
		* - paddle, ball, display	
		*/
		window.clear();
		window.draw(hud);
		window.draw(paddle.getShape());
		window.draw(ball.getShape());
		window.display();

	}

	return 0;
}
