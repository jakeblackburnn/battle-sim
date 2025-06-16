//
// Created By J. Blackburn - Mar 22 2025
//

#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <vector>
#include <unordered_map>
#include <iostream>
using namespace std;

	// implementation data structures
#include "Renderer.h"
class    EventHandler; // forward declaration of Event Handler
		     
enum class GameState { PLACING, RUNNING, WON, LOST };

	// Game space data structures
#include "battlefield.h"
#include "combatants/Combatant.h"
#include "combatants/Attack.h"
#include "combatants/Ranged.h"
#include "combatants/Intel.h"
	
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

	bool isOccupied(Position p);


		// Core Game State
	GameState state;
	bool      running;
	int       tics;
	int       maxTics;

		// Placement Mode State
	Color currentPlaceType;
	bool      eraseMode;

		// battlefield state
	Battlefield battlefield;

		// UI elements
	SDL_Rect playButtonRect;
	SDL_Rect nextButtonRect;
	SDL_Rect typeButtonRects[3];
	SDL_Rect eraseButtonRect;
	SDL_Rect battlefieldRect;

		// Audio 
	Mix_Music* battleMusic;
	Mix_Chunk* clickSound;

		// Fonts
	TTF_Font* font;

private:

		// Core Impl Architecture
	SDL_Window*   window;
	Renderer*     renderer;
	EventHandler* eventHandler;


		// Helper Functions
	void addCombatant(Position p, Color c);
	void deleteCombatant(Combatant* combatant);


};

#endif
