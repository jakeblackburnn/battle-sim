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
			// troop type buttons
		for (int i = 0; i < 3; i++) {
			if ( mx >= game->typeButtonRects[i].x && 
			     mx <= game->typeButtonRects[i].x + game->typeButtonRects[i].w &&
                	     my >= game->typeButtonRects[i].y && 
			     my <= game->typeButtonRects[i].y + game->typeButtonRects[i].h ) 
			{
				if (i == 0) game->currentPlaceType = TroopType::RED;
				if (i == 1) game->currentPlaceType = TroopType::ORANGE;
				if (i == 2) game->currentPlaceType = TroopType::YELLOW;
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
		  if ( mx > 351 && my > 251 && mx <= 651 && my <= 551) { //TODO: figure out battlefield boundaries
		        
		          if (game->eraseMode) { // attempt erase troop

				  // TODO: figure out troop erasure


			  } else { // attempt to place troop
				   
				// figure out troop coordinates
				
				  int bx = mx - 350;
				  int by = my - 250;

				  int tx = (bx - (bx % 3)) / 3;
				  int ty = (by - (by % 3)) / 3;

				  std::cout << "Attempting to place troop at: " << tx << ", " << ty << std::endl;
				   
					  // check if spot is occupied

				  if (!game->isOccupied(tx, ty)) {
					  // place troop (use game helper func)
					  if (game->currentPlaceType == TroopType::RED) {
						  game->red.push_back( Troop(tx, ty, TroopType::RED) );
					  }
					  if (game->currentPlaceType == TroopType::ORANGE) {
						  game->orange.push_back( Troop(tx, ty, TroopType::ORANGE) );
					  }
					  if (game->currentPlaceType == TroopType::YELLOW) {
						  game->yellow.push_back( Troop(tx, ty, TroopType::YELLOW) );
					  }

					  return;
				  }
			  }
		  }
        }
}


void EventHandler::handleSimulationEvent(SDL_Event& e) {
}
