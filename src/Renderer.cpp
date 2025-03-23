//
// Created by J. Blackburn - Mar 23 2025
//


#include "Renderer.h"

Renderer::Renderer(SDL_Renderer* sdlRenderer) : renderer(sdlRenderer) {}

Renderer::~Renderer() {
	SDL_DestroyRenderer(renderer);
}


void Renderer::presentScreen() {
	SDL_RenderPresent(renderer);
}

void Renderer::clearScreen() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // set draw color to white
	SDL_RenderClear(renderer);  
}


void Renderer::renderBattlefield( const TroopVector red, 
				  const TroopVector orange, 
				  const TroopVector yellow, 
				  const TroopVector purple, 
				  const TroopVector blue, 
				  const TroopVector green  )
{
	renderTroops(red,    TroopType::RED);
	renderTroops(purple, TroopType::PURPLE);

	renderTroops(orange, TroopType::ORANGE);
	renderTroops(blue,   TroopType::BLUE);

	renderTroops(yellow, TroopType::YELLOW);
	renderTroops(green,  TroopType::GREEN);
}


void Renderer::renderTroops( const TroopVector troops, TroopType type ) {

	for ( const auto& troop : troops ) {
			// set render position
		SDL_Rect rect;
		rect.x = troop.x;
		rect.y = troop.y;
		rect.w = 1;
		rect.h = 1;
			// Set Friendly Colors
		if (type == TroopType::RED)    { SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); }
		if (type == TroopType::ORANGE) { SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255); }
		if (type == TroopType::YELLOW) { SDL_SetRenderDrawColor(renderer, 255, 222, 33, 255); }
			// Set Enemy Colors
		if (type == TroopType::PURPLE) { SDL_SetRenderDrawColor(renderer, 100, 10, 170, 255); }
		if (type == TroopType::BLUE)   { SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); }
		if (type == TroopType::GREEN)  { SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); }
			// draw troop
		SDL_RenderFillRect(renderer, &rect);
	}
}



void Renderer::renderUI( bool  isPlacing, 
		         bool  eraseMode, 
		         int   budget, 

		         const SDL_Rect& playButtonRect,
		         const SDL_Rect& nextButtonRect,
		         const SDL_Rect& eraseButtonRect,
		         const SDL_Rect* typeButtonRects  ) 
{
		// Render Play button
	SDL_SetRenderDrawColor(renderer, 50, 255, 50, 255); // light green play button
	SDL_RenderDrawRect(renderer, &playButtonRect);

		// Render troop placement type selection
	for (int i = 0; i < 3; i++) {
		SDL_Rect typeRect = typeButtonRects[i];
		if      (i == 0) { SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); }
		else if (i == 1) { SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255); }
		else if (i == 2) { SDL_SetRenderDrawColor(renderer, 255, 222, 33, 255); }

		SDL_RenderFillRect(renderer, &typeRect);
		SDL_RenderDrawRect(renderer, &typeRect); // button outline
	}

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderFillRect(renderer, &eraseButtonRect);
	SDL_RenderDrawRect(renderer, &eraseButtonRect);
}

void Renderer::renderGameOverMessage(bool won) {}

