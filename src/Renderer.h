// 
// Created by J. Blackburn - Mar 22 2025
//

#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <vector>
#include "Troop.h"

	// vector of troops type alias
using TroopVector = std::vector<Troop>&;

class Renderer {
public:
	 Renderer(SDL_Renderer* sdlRenderer);
	~Renderer();

	void renderBattlefield( TroopVector red, 
			        TroopVector orange, 
				TroopVector yellow, 
				TroopVector purple, 
				TroopVector blue, 
				TroopVector green  );

	void renderUI( bool  isPlacing, 
		       bool  eraseMode, 
		       int   Budget, 
		       TroopType currentPlaceType,

		       const SDL_Rect& playButtonRect,
		       const SDL_Rect& nextButtonRect,
		       const SDL_Rect& eraseButtonRect,
		       const SDL_Rect* typeButtonRects  );


	void renderGameOverMessage(bool won);
	void clearScreen();
	void presentScreen();

private:
	SDL_Renderer* renderer;
	void renderTroops(TroopVector troops, TroopType type);

};

#endif
