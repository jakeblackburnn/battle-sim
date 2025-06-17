//
// Created by J. Blackburn - Mar 22 2025
//

#include "EventHandler.h"
#include "Game.h"
#include <iostream>

EventHandler::EventHandler(Game* game) : game(game) {}

EventHandler::~EventHandler() {}

void EventHandler::handleEvent(SDL_Event& e) {

	if (e.type == SDL_QUIT) {
		game->setRunning(false);
		return;
	}

	if (game->getState() == GameState::RUNNING) {
		handleSimulationEvent(e);
	} else {
		handlePlacementEvent(e);
	}
}


void EventHandler::handlePlacementEvent(SDL_Event& e) {
	
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		int mx = e.button.x;
		int my = e.button.y;
			// play button clicked
		if ( mx >= game->playButtonRect.x && 
		     mx <= game->playButtonRect.x + game->playButtonRect.w &&
            	     my >= game->playButtonRect.y && 
		     my <= game->playButtonRect.y + game->playButtonRect.h )
		{
			game->state = GameState::RUNNING;
			if (game->battleMusic) Mix_PlayMusic(game->battleMusic, -1);
			return;
		}


			// reset button clicked
		if (mx >= game->resetButtonRect.x &&
		    mx <= game->resetButtonRect.x + game->resetButtonRect.w &&
		    my >= game->resetButtonRect.y &&
		    my <= game->resetButtonRect.y + game->resetButtonRect.h)
		{
		    game->setupLevel(); // resets the current level
		    return;
		}


			// troop type buttons
		for (int i = 0; i < 3; i++) {
			if ( mx >= game->typeButtonRects[i].x && 
			     mx <= game->typeButtonRects[i].x + game->typeButtonRects[i].w &&
                	     my >= game->typeButtonRects[i].y && 
			     my <= game->typeButtonRects[i].y + game->typeButtonRects[i].h ) 
			{
				if (i == 0) game->currentPlaceType = Color::RED;
				if (i == 1) game->currentPlaceType = Color::ORANGE;
				if (i == 2) game->currentPlaceType = Color::YELLOW;
				game->eraseMode = false;
				return;
			}
		}
			// erase button
		  if ( mx >= game->eraseButtonRect.x && 
		       mx <= game->eraseButtonRect.x + game->eraseButtonRect.w &&
                       my >= game->eraseButtonRect.y && 
		       my <= game->eraseButtonRect.y + game->eraseButtonRect.h ) 
		  { 
			  game->eraseMode = true;
		          return; 
		  }

		  	// battlefield placement
		  if (false) {  // TODO: figure put troop placement
		        
		          if (game->eraseMode) { // attempt erase troop

				  // TODO: figure out troop erasure


			  } else { // attempt to place troop
				
				  int bx = mx - 350; // battlefield-wise coords
				  int by = my - 250;

				  int tx = (bx - (bx % 3)) / 3; // troop position coords
				  int ty = (by - (by % 3)) / 3;

				  std::cout << "Attempting to place troop at: " << tx << ", " << ty << std::endl;
				   
					  // check if spot is occupied

				  if ( !game->isOccupied( Position(tx, ty) )) {
					  // place troop (use game helper func)
					  // TODO: figure out troop instantiation
					  if (game->currentPlaceType == Color::RED) {
					  }
					  if (game->currentPlaceType == Color::ORANGE) {
					  }
					  if (game->currentPlaceType == Color::YELLOW) {
					  }

					  return;
				  }
			  }
		  }
        }
}


void EventHandler::handleSimulationEvent(SDL_Event& e) {
}
