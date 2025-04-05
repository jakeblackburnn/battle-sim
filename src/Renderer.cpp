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


void Renderer::renderBattlefield( CombatantVec red, 
				  CombatantVec orange, 
				  CombatantVec yellow, 
				  CombatantVec purple, 
				  CombatantVec blue, 
				  CombatantVec green,  
				  const SDL_Rect& battlefieldRect )
{

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black border around battlefield
	SDL_RenderDrawRect(renderer, &battlefieldRect);

	int startX = battlefieldRect.x;
	int startY = battlefieldRect.y;

	renderCombatants(red,    Color::RED,    startX, startY);
	renderCombatants(purple, Color::PURPLE, startX, startY);

	renderCombatants(orange, Color::ORANGE, startX, startY);
	renderCombatants(blue,   Color::BLUE,   startX, startY);

	renderCombatants(yellow, Color::YELLOW, startX, startY);
	renderCombatants(green,  Color::GREEN,  startX, startY);
}


void Renderer::renderCombatants( CombatantVec combatants, Color color, int startX, int startY) {

	for ( const auto& combatant : combatants ) {
			// set render position
		SDL_Rect rect;
		Position p = combatant->getPosition();
		rect.x = startX + p.x * 3;
		rect.y = startY + p.y * 3;
		rect.w = 5;
		rect.h = 5;
			// Set Friendly Colors
		if (color == Color::RED)    { SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); }
		if (color == Color::ORANGE) { SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255); }
		if (color == Color::YELLOW) { SDL_SetRenderDrawColor(renderer, 255, 222, 33, 255); }
			// Set Enemy Colors
		if (color == Color::PURPLE) { SDL_SetRenderDrawColor(renderer, 100, 10, 170, 255); }
		if (color == Color::BLUE)   { SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); }
		if (color == Color::GREEN)  { SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); }
			// draw troop
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &rect); // black border around troop
	}
}



void Renderer::renderUI( bool  isPlacing, 
		         bool  eraseMode, 
			 Color currentPlaceType,

		         const SDL_Rect& playButtonRect,
		         const SDL_Rect& nextButtonRect,
		         const SDL_Rect& eraseButtonRect,
		         const SDL_Rect* typeButtonRects  ) 
{
		// Render Play button
	SDL_SetRenderDrawColor(renderer, 50, 255, 50, 255); // light green play button
	SDL_RenderFillRect(renderer, &playButtonRect);

		// Render troop placement type selection
	for (int i = 0; i < 3; i++) {
		SDL_Rect typeRect = typeButtonRects[i];
		if      (i == 0) { SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); }
		else if (i == 1) { SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255); }
		else if (i == 2) { SDL_SetRenderDrawColor(renderer, 255, 222, 33, 255); }

		SDL_RenderFillRect(renderer, &typeRect);

			// green border around current selection
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		if (i == 0 && currentPlaceType == Color::RED) {
			SDL_RenderDrawRect(renderer, &typeRect);
		}
		if (i == 1 && currentPlaceType == Color::ORANGE) {
			SDL_RenderDrawRect(renderer, &typeRect);
		}
		if (i == 2 && currentPlaceType == Color::YELLOW) {
			SDL_RenderDrawRect(renderer, &typeRect);
		}
							 
	}

	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderFillRect(renderer, &eraseButtonRect);
	SDL_RenderDrawRect(renderer, &eraseButtonRect);

	if (eraseMode) { 
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(renderer, &eraseButtonRect);
	}
} 

void Renderer::renderGameOverMessage(bool won) {}

