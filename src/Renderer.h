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

	void RenderBattleField( const TroopVector red, 
			        const TroopVector orange, 
				const TroopVector yellow, 
				const TroopVector purple, 
				const TroopVector, blue, 
				const TroopVector green  );

	void RenderUI( bool  isPlacing, 
		       bool  eraseMode, 
		       int   remainingBudget, 

		       const SDL_Rect& playButtonRect,
		       const SDL_Rect& nextButtonRect,
		       const SDL_Rect& eraseButtonRect,
		       const SDL_Rect* typeButtonRects  );


	renderGameOverMessage(bool won);
	void clearScreen();
	void presentScreen();

private:
	SDL_Renderer* renderer;
	void renderTroopVector(const TroopVector troops);

};

#endif
