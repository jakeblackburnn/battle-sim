//
// Created By J. Blackburn - Mar 22 2025
//

#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "Troop.h"
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
	bool      isBudgetEmpty() const {
		if (budget == 0) return true;
		else return false;
	}
	GameState getState()  const { return state; }

		// Freindlies
	std::vector<Troop> red;
	std::vector<Troop> orange;
	std::vector<Troop> yellow;

		// Enemies
	std::vector<Troop> purple;
	std::vector<Troop> blue;
	std::vector<Troop> green;

	bool isOccupied(int x, int y);


		// Core Game State
	GameState state;
	bool      running;
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
	Mix_Music* battleMusic;
	Mix_Chunk* clickSound;

private:

	SDL_Window*   window;
	Renderer* renderer;
	EventHandler* eventHandler;

		// Helper Functions
	void addFriendly(int x, int y, TroopType type);
	void removeFriendly(int x, int y);

	void move();
	MoveOption selectMove(int fr, int f, int fl, int r, int h, int l, int br, int b, int bl, int s);

	void kill();
	bool selectKill(int l, int d);
	void deleteTroop(TroopVector troops, Troop* t);

	int countFrontFriendlys(int x, int y); 
	int countBackFriendlys (int x, int y); 
	int countLeftFriendlys (int x, int y);
	int countRightFriendlys(int x, int y);

	int countClosePurples(int x, int y);
	int countMidPurples  (int x, int y); 
	int countMidBlues    (int x, int y); 
	int countLongBlues   (int x, int y); 
	int countMidGreens   (int x, int y); 

	int countCloseReds(int x, int y);
	int countMidReds  (int x, int y); 
	int countMidOranges    (int x, int y); 
	int countLongOranges   (int x, int y); 
	int countMidYellows   (int x, int y); 
};

#endif
