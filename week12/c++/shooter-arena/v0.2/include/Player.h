#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {

	// private
	private:
		// player's initial health and speed status
		const float initial_Speed = 200; 
		const float initial_Health = 100; 

		// locate player
		Vector2f m_Posn; 
		// player sprite
		Sprite m_Sprite;
		// player texture
		Texture m_Texture;
		// screen resolution
		Vector2f m_ScreenRes; 
		// arena size
		IntRect m_Arena; 
		// size per tile
		int m_TileSize;
		// player's current movement
		bool m_UpActive; 
		bool m_DownActive; 
		bool m_LeftActive; 
		bool m_RightActive; 
		// current health status
		int m_HealthStatus; 
		int m_HealthMax; 
		// time player last hit
		Time m_LastHit;
		// speed - pixels ps
		float m_Speed;
		
	// public functions
	public:
	
		Player();

		// set as player spawned
		void spawn(IntRect arena, Vector2f screenRes, int tileSize);	
		// call at end of game
		void resetPlayerStats();
		// player hit by mob object
		bool hit(Time timeHit);
		// when was player last hit
		Time getTimeLastHit(); 
		// check player's posn
		FloatRect getPlayerPosn(); 
		// check player's centre
		Vector2f getPlayerCentre(); 
		// check player's rotation
		float getPlayerRotation(); 
		// get player's sprite
		Sprite getPlayerSprite(); 
		
		// player motion
		void motionUp(); 
		void motionDown(); 
		void motionLeft(); 
		void motionRight(); 

		// player stop motion
		void stopUp();
		void stopDown();
		void stopLeft();
		void stopRight();

		// update per frame
		void update(float timeElapsed, Vector2i mousePosn); 
		
		// player's speed bonus
		void bonusSpeed(); 
		// player's health bonus
		void bonusHealth(); 

		// increase player's max health
		void incrHealthMax(int amount); 

		// check player's current health
		int getPlayerHealth(); 
		
};
