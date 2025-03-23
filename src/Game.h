//
// Created By J. Blackburn - Mar 22 2025
//

#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "Troop.h"

class EventHandler;

	// Game State enum
enum class GameState { PLACING, RUNNING, WON, LOST };
	
	// Game class

class Game {
public: 
 	 Game();
	~Game();

	bool init();
	void cleanUp();

	void setupLevel(int level);

	void update();
	void render();
	void handleEvent(SDL_Event& e);

	void      setRunning(bool flag);
	bool      isRunning() const { return running; }
	GameState getState()  const { return state; }

private:

	SDL_Window*   window;
	SDL_Renderer* renderer;
	EventHandler* eventHandler;

		// Freindlies
	std::vector<Troop> Red;
	std::vector<Troop> Orange;
	std::vector<Troop> Yellow;

		// Enemies
	std::vector<Troop> Purple;
	std::vector<Troop> Blue;
	std::vector<Troop> Green;


		// Core Game State
	GameState state;
	bool      running;
	int       currentLevel;
	int       tics;
	int       maxTics;

		// Placement Mode State
	int       budget;
	TroopType currentPlaceType;
	bool      eraseMode;

		// UI elements
	SDL_Rect playButtonRect;
	SDL_Rect nextButtonRect;
	SDL_Rect typeButtonRects[3];
	SDL_Rect eraseButtonRect;

		// Audio 
	Mix_Chunk* battleMusic;
	Mix_Chunk* clickSound;



		// Helper Functions
	void addFriendly(int x, int y, TroopType type);
	void removeFriendly(int x, int y);
};

#endif
