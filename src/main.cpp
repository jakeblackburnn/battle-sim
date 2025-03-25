//
// Created by J. Blackburn - Mar 22 2025
//

#include "Game.h"
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {

	Game game;

	if (!game.init()) {
		std::cerr << "Failed to initialize Game." << std::endl;
		return -1;
	}

	game.setupLevel();


		// MAIN GAME LOOP
	SDL_Event e;

	while (game.isRunning()) {
		
		while (SDL_PollEvent(&e) != 0) {
			game.handleEvent(e);
		}

		if (game.getState() == GameState::RUNNING) {
			game.update();
		}

		game.render();

		SDL_Delay(5); // 32 ms delay between tics
	}


	game.cleanUp();

	return 0;
}
