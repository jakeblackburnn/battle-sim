// 
// Created by J. Blackburn - Mar 22 2025
//

#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL.h>

class Game;

class EventHandler {
public:

	 EventHandler(Game* game);
	~EventHandler();

	void handleEvent(SDL_Event& e);

private:
	Game* game;

	void handlePlacementEvent(SDL_Event& e);
	void handleSimulationEvent(SDL_Event& e);
};

#endif
