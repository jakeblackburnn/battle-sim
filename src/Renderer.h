// 
// Created by J. Blackburn - Mar 22 2025
//

#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <vector>

#include "combatants/Combatant.h"
#include "combatants/Attack.h"

using CombatantVec = std::vector<Combatant*>&;

class Renderer {
public:
	 Renderer(SDL_Renderer* sdlRenderer);
	~Renderer();

	void renderBattlefield( CombatantVec red,
				CombatantVec orange,
				CombatantVec yellow,
				CombatantVec purple,
				CombatantVec blue,
				CombatantVec green,
				const SDL_Rect& battlefieldRect ); 

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
	void renderCombatants( CombatantVec combatants, Color color, int startX, int startY); 

};

#endif
