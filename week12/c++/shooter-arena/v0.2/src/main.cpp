#include <SFML/Graphics.hpp>
#include "Arena.h"
#include "Player.h"

using namespace sf;

int main() {
	// game states
	// - one of four is always active
	enum class State { PAUSED, LEVEL_UP, GAME_OVER, PLAYING };
	// set initial state for game
	State state = State::GAME_OVER;

	/* 
	* create SFML window
	*/
	// set resolution of game window
	Vector2f screenRes;
	screenRes.x = VideoMode::getDesktopMode().width;
	screenRes.y = VideoMode::getDesktopMode().height;
	// render game window
	RenderWindow window(VideoMode(screenRes.x, screenRes.y),
		"Mummies", Style::Fullscreen);

	// create SFML View for main game usage
	View mainView(sf::FloatRect(0, 0, screenRes.x, screenRes.y));

	// clock for timing 
	Clock clock;
	// record duration PLAYING state active
	Time gameTotalTime;

	// mouse relative to world coordinates
	Vector2f mouseWorldPosn;
	// mouse relative to screen coordinates
	Vector2i mouseScreenPosn;

	// instantiate player
	Player player;

	// define boundaries of game arena
	IntRect arena;

	// background
	VertexArray background;
	Texture textureBg;
	// load sprite sheet
	textureBg.loadFromFile("../assets/graphics/background_sheet.png");

	// main game loop
	while (window.isOpen()) {

		/*
		* Input
		*/
		Event event;
		// poll for events
		while (window.pollEvent(event)) {

			if (event.type == Event::KeyPressed) {
				// pause game during gameplay
				if (event.key.code == Keyboard::Return && state == State::PLAYING) {
					state = State::PAUSED;
				} // toggle restart
				else if (event.key.code == Keyboard::Return && state == State::PAUSED) {
					state = State::PLAYING;
					// reset clock, avoids frame jump
					clock.restart();
				} // start new game
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) {
					state = State::LEVEL_UP;
				}

				if (state == State::PLAYING) {

				}

			}
		} // end of polling

		// handle quitting game window
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		// handle events for gameplay - wasd layout
		if (state == State::PLAYING) {
			// handle wasd key press and release
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player.motionUp();
			} else {
				player.stopUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				player.motionDown();
			} else {
				player.stopDown();
			}
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				player.motionLeft();
			} else {
				player.stopLeft();
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				player.motionRight();
			} else {
				player.stopRight();
			}
		}

		// handle level up state - change by player
		if (state == State::LEVEL_UP) {
			if (event.key.code == Keyboard::Num1) {
				state = State::PLAYING;
			}

			// check state is PLAYING
			if (state == State::PLAYING) {
				// set the level
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				// pass VertexArray by reference
				int tileSize = generateBackground(background, arena);
				
				// spawn player in middle of arena
				player.spawn(arena, screenRes, tileSize);
				// reset clock, avoids frame jump
				clock.restart();
			}
		} // end level up

		/*
		* Update the frame
		*/
		if (state == State::PLAYING) {
			// update delta time
			Time dt = clock.restart();
			// update total time for game
			gameTotalTime += dt;						
			// get fraction from delta time
		  float dtAsSeconds = dt.asSeconds();
			
			// check posn of mouse pointer
		  mouseScreenPosn = Mouse::getPosition();
			// update world coords for main view
			mouseWorldPosn = window.mapPixelToCoords(Mouse::getPosition(), mainView);	
			
			// update player
			player.update(dtAsSeconds, Mouse::getPosition());
			// record new player posn
			Vector2f playerPosition(player.getPlayerCentre());
			// centre view around player
			mainView.setCenter(player.getPlayerCentre());
		} // end of frame update

		/*
		* Draw the scene
		*/
		if (state == State::PLAYING) {
			window.clear();
			// set main view to render in game window
			window.setView(mainView);
			// draw sprite sheet background
			window.draw(background, &textureBg);
			// draw player
			window.draw(player.getPlayerSprite());
		}

		if (state == State::LEVEL_UP) {
		}

		if (state == State::PAUSED) {
		}

		if (state == State::GAME_OVER) {
		}

		window.display();

	}// end game loop

	return 0;
}
