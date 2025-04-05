// 
// Created by J. Blackburn - Mar 22 2025
//

#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

#include "battlefield.h"
#include "combatants/Combatant.h"
#include "combatants/Attack.h"


class Renderer {
public:
	 Renderer(SDL_Renderer* sdlRenderer);
	~Renderer();

	void renderBattlefield( Battlefield battlefield, const SDL_Rect& battlefieldRect ); 

	void renderUI( bool  isPlacing, 
		       bool  eraseMode, 
		       Color currentPlaceType,

		       const SDL_Rect& playButtonRect,
		       const SDL_Rect& nextButtonRect,
		       const SDL_Rect& eraseButtonRect,
		       const SDL_Rect* typeButtonRects );


	void renderGameOverMessage(bool won);
	void clearScreen();
	void presentScreen();

private:
	SDL_Renderer* renderer;
};

#endif
