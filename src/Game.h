//
// Created By J. Blackburn - Mar 22 2025
//

#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>

#include "Renderer.h"

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

	void setupLevel();

	void update();
	void render();
	void handleEvent(SDL_Event& e);

	void      setRunning(bool flag);
	bool      isRunning() const { return running; }
	GameState getState()  const { return state; }

		// Freindlies

		// Enemies

	bool isOccupied(int x, int y);


		// Core Game State
	GameState state;
	bool      running;
	int       tics;
	int       maxTics;

		// Placement Mode State
	TroopType currentPlaceType;
	bool      eraseMode;

		// UI elements
	SDL_Rect playButtonRect;
	SDL_Rect nextButtonRect;
	SDL_Rect typeButtonRects[3];
	SDL_Rect eraseButtonRect;
	// SDL_Rect battlefieldRect;

		// Audio 
	Mix_Music* battleMusic;
	Mix_Chunk* clickSound;

private:

		// Core Architecture
	SDL_Window*   window;
	Renderer*     renderer;
	EventHandler* eventHandler;

		// Helper Functions
	placeCombatant(Position p, Color c);
	removeCombatant(Combatant* combatant);


};

#endif
