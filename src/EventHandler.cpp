//
// Created by J. Blackburn - Mar 22 2025
//

#include "EventHandler.h"
#include "Game.h"

EventHandler::EventHandler(Game* game) : game(game) {}

EventHandler::~EventHandler() {}

void EventHandler::handleEvent(SDL_Event& e) {

	if (e.type == SDL_QUIT) {
		game->setRunning(false);
		return;
	}

	if (game->getState() == GameState::PLACING) {
		handlePlacementEvent(e);
	}

	if (game->getState() == GameState::RUNNING) {
		handleSimulationEvent(e);
	}
}


void EventHandler::handlePlacementEvent(SDL_Event& e) {
	
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		int mx = e.button.x;
		int my = e.button.y;

		// ensure click is within bounds
		// if (mx >= 0 && mx <= 600 && my >= 50 && my <= 650) {}
		// place or erase troops
	}
}


void EventHandler::handleSimulationEvent(SDL_Event& e) {
}
