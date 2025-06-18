//
// Created by J. Blackburn - Mar 22 2025
//

#include "Game.h"
#include "EventHandler.h"

#include <iostream>
#include <stdlib.h>
#include <random>

Game::Game() : window(nullptr), 
	       renderer(nullptr), 
	       eventHandler(nullptr),
	       state(GameState::PLACING), 
	       running(false), 
	       tics(0), 
	       maxTics(10000),
	       currentPlaceType(Color::RED),
	       eraseMode(false),
	       clickSound(nullptr),
	       battleMusic(nullptr),
	       font(nullptr)
{


	playButtonRect =  { 400, 20, 60, 30 };
	resetButtonRect = { 500, 20, 75, 30 };

	battlefieldRect = { 349, 249, 303, 303 };

	for (int y = 0; y < 3; y++) {
		typeButtonRects[y] = { 25, 50 + y*40, 30, 30 };
	}

	eraseButtonRect = { 25, 50 + 3*40, 30, 30 };


}


bool Game::init() {

		// initialize SDL text rendering
	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
		return false;
	}

	font = TTF_OpenFont("../assets/Roboto_Condensed-Regular.ttf", 24);
	if (!font) {
		std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
		return false;
	}

		// Initialize SDL video and audio
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        	std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
		return false;
        }
		// Create Window
	window = SDL_CreateWindow("Battle Sim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, SDL_WINDOW_SHOWN);
	if (!window) {
		std::cerr << "Window Creation Failed." << SDL_GetError() << std::endl;
		return false;
	}
		// Create Renderer
	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!sdlRenderer) {
		std::cerr << "Renderer Creation Failed." << SDL_GetError() << std::endl;
		return false;
	}
	renderer = new Renderer(sdlRenderer);

		// Create Renderer
	eventHandler = new EventHandler(this);

		// initialize audio
	int audio_rate = 44100;
	Uint16 audio_format = MIX_DEFAULT_FORMAT;
	int audio_channels = 2;
	int audio_chunksize = 2048;

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_chunksize) < 0) {
		std::cerr << "SDL Mixer Failed." << Mix_GetError() << std::endl;
		return false;
	}

		// load audio files 
	// fxClick = Mix_LoadWAV("assets/asdf");
	// battleMusic = Mix_LoadMUS("assets/asdf");
	// if (!fxClick || !battleMusic) {
	// 	std::cerr << "Failed to Load Audio files" << Mix_GetError() << std::endl;
	// 	return false;
	// }
	
	running = true;
	return true;
}

void Game::setRunning(bool flag) {
	running = flag;
}

void Game::setupLevel() {

		// clear battlefield
	for (auto& [p, dat] : battlefield) {
		if (dat.occupant) {
			// TODO: implement combatant destructor
			// delete dat.occupant;
			dat.occupant = nullptr;
		}
	}
	battlefield.clear();

		// Place Enemy troops
	addCombatant( Position(75, 10), Color::PURPLE );
	addCombatant( Position(75, 15), Color::PURPLE );
	addCombatant( Position(75, 20), Color::PURPLE );
	addCombatant( Position(75, 25), Color::PURPLE );
	addCombatant( Position(75, 30), Color::PURPLE );
	addCombatant( Position(75, 35), Color::PURPLE );
	addCombatant( Position(75, 40), Color::PURPLE );
	addCombatant( Position(75, 45), Color::PURPLE );
	addCombatant( Position(75, 50), Color::PURPLE );
	addCombatant( Position(75, 55), Color::PURPLE );
	addCombatant( Position(75, 60), Color::PURPLE );
	addCombatant( Position(75, 65), Color::PURPLE );
	addCombatant( Position(75, 70), Color::PURPLE );
	addCombatant( Position(75, 75), Color::PURPLE );
	addCombatant( Position(75, 80), Color::PURPLE );
	addCombatant( Position(75, 85), Color::PURPLE );
	addCombatant( Position(75, 90), Color::PURPLE );
	addCombatant( Position(75, 95), Color::PURPLE );

	addCombatant( Position(70, 10), Color::PURPLE );
	addCombatant( Position(70, 15), Color::PURPLE );
	addCombatant( Position(70, 20), Color::PURPLE );
	addCombatant( Position(70, 25), Color::PURPLE );
	addCombatant( Position(70, 30), Color::PURPLE );
	addCombatant( Position(70, 35), Color::PURPLE );
	addCombatant( Position(70, 40), Color::PURPLE );
	addCombatant( Position(70, 45), Color::PURPLE );
	addCombatant( Position(70, 50), Color::PURPLE );
	addCombatant( Position(70, 55), Color::PURPLE );
	addCombatant( Position(70, 60), Color::PURPLE );
	addCombatant( Position(70, 65), Color::PURPLE );
	addCombatant( Position(70, 70), Color::PURPLE );
	addCombatant( Position(70, 75), Color::PURPLE );
	addCombatant( Position(70, 80), Color::PURPLE );
	addCombatant( Position(70, 85), Color::PURPLE );
	addCombatant( Position(70, 90), Color::PURPLE );
	addCombatant( Position(70, 95), Color::PURPLE );

	addCombatant( Position(80, 10), Color::BLUE );
	addCombatant( Position(80, 15), Color::BLUE );
	addCombatant( Position(80, 20), Color::BLUE );
	addCombatant( Position(80, 25), Color::BLUE );
	addCombatant( Position(80, 30), Color::BLUE );
	addCombatant( Position(80, 35), Color::BLUE );
	addCombatant( Position(80, 40), Color::BLUE );
	addCombatant( Position(80, 45), Color::BLUE );
	addCombatant( Position(80, 50), Color::BLUE );
	addCombatant( Position(80, 55), Color::BLUE );
	addCombatant( Position(80, 60), Color::BLUE );
	addCombatant( Position(80, 65), Color::BLUE );
	addCombatant( Position(80, 70), Color::BLUE );
	addCombatant( Position(80, 75), Color::BLUE );
	addCombatant( Position(80, 80), Color::BLUE );
	addCombatant( Position(80, 85), Color::BLUE );
	addCombatant( Position(80, 90), Color::BLUE );
	addCombatant( Position(80, 95), Color::BLUE );

	addCombatant( Position(85, 10), Color::GREEN );
	addCombatant( Position(85, 15), Color::GREEN );
	addCombatant( Position(85, 20), Color::GREEN );
	addCombatant( Position(85, 25), Color::GREEN );
	addCombatant( Position(85, 30), Color::GREEN );
	addCombatant( Position(85, 35), Color::GREEN );
	addCombatant( Position(85, 40), Color::GREEN );
	addCombatant( Position(85, 45), Color::GREEN );
	addCombatant( Position(85, 50), Color::GREEN );
	addCombatant( Position(85, 55), Color::GREEN );
	addCombatant( Position(85, 60), Color::GREEN );
	addCombatant( Position(85, 65), Color::GREEN );
	addCombatant( Position(85, 70), Color::GREEN );
	addCombatant( Position(85, 75), Color::GREEN );
	addCombatant( Position(85, 80), Color::GREEN );
	addCombatant( Position(85, 85), Color::GREEN );
	addCombatant( Position(85, 90), Color::GREEN );
	addCombatant( Position(85, 95), Color::GREEN );

		// Place Friendly troops
	addCombatant( Position(25, 10), Color::RED );
	addCombatant( Position(25, 15), Color::RED );
	addCombatant( Position(25, 20), Color::RED );
	addCombatant( Position(25, 25), Color::RED );
	addCombatant( Position(25, 30), Color::RED );
	addCombatant( Position(25, 35), Color::RED );
	addCombatant( Position(25, 40), Color::RED );
	addCombatant( Position(25, 45), Color::RED );
	addCombatant( Position(25, 50), Color::RED );
	addCombatant( Position(25, 55), Color::RED );
	addCombatant( Position(25, 60), Color::RED );
	addCombatant( Position(25, 65), Color::RED );
	addCombatant( Position(25, 70), Color::RED );
	addCombatant( Position(25, 75), Color::RED );
	addCombatant( Position(25, 80), Color::RED );
	addCombatant( Position(25, 85), Color::RED );
	addCombatant( Position(25, 90), Color::RED );
	addCombatant( Position(25, 95), Color::RED );

	addCombatant( Position(30, 10), Color::RED );
	addCombatant( Position(30, 15), Color::RED );
	addCombatant( Position(30, 20), Color::RED );
	addCombatant( Position(30, 25), Color::RED );
	addCombatant( Position(30, 30), Color::RED );
	addCombatant( Position(30, 35), Color::RED );
	addCombatant( Position(30, 40), Color::RED );
	addCombatant( Position(30, 45), Color::RED );
	addCombatant( Position(30, 50), Color::RED );
	addCombatant( Position(30, 55), Color::RED );
	addCombatant( Position(30, 60), Color::RED );
	addCombatant( Position(30, 65), Color::RED );
	addCombatant( Position(30, 70), Color::RED );
	addCombatant( Position(30, 75), Color::RED );
	addCombatant( Position(30, 80), Color::RED );
	addCombatant( Position(30, 85), Color::RED );
	addCombatant( Position(30, 90), Color::RED );
	addCombatant( Position(30, 95), Color::RED );

	addCombatant( Position(20, 10), Color::ORANGE );
	addCombatant( Position(20, 15), Color::ORANGE );
	addCombatant( Position(20, 20), Color::ORANGE );
	addCombatant( Position(20, 25), Color::ORANGE );
	addCombatant( Position(20, 30), Color::ORANGE );
	addCombatant( Position(20, 35), Color::ORANGE );
	addCombatant( Position(20, 40), Color::ORANGE );
	addCombatant( Position(20, 45), Color::ORANGE );
	addCombatant( Position(20, 50), Color::ORANGE );
	addCombatant( Position(20, 55), Color::ORANGE );
	addCombatant( Position(20, 60), Color::ORANGE );
	addCombatant( Position(20, 65), Color::ORANGE );
	addCombatant( Position(20, 70), Color::ORANGE );
	addCombatant( Position(20, 75), Color::ORANGE );
	addCombatant( Position(20, 80), Color::ORANGE );
	addCombatant( Position(20, 85), Color::ORANGE );
	addCombatant( Position(20, 90), Color::ORANGE );
	addCombatant( Position(20, 95), Color::ORANGE );

	addCombatant( Position(15, 10), Color::YELLOW );
	addCombatant( Position(15, 15), Color::YELLOW );
	addCombatant( Position(15, 20), Color::YELLOW );
	addCombatant( Position(15, 25), Color::YELLOW );
	addCombatant( Position(15, 30), Color::YELLOW );
	addCombatant( Position(15, 35), Color::YELLOW );
	addCombatant( Position(15, 40), Color::YELLOW );
	addCombatant( Position(15, 45), Color::YELLOW );
	addCombatant( Position(15, 50), Color::YELLOW );
	addCombatant( Position(15, 55), Color::YELLOW );
	addCombatant( Position(15, 60), Color::YELLOW );
	addCombatant( Position(15, 65), Color::YELLOW );
	addCombatant( Position(15, 70), Color::YELLOW );
	addCombatant( Position(15, 75), Color::YELLOW );
	addCombatant( Position(15, 80), Color::YELLOW );
	addCombatant( Position(15, 85), Color::YELLOW );
	addCombatant( Position(15, 90), Color::YELLOW );
	addCombatant( Position(15, 95), Color::YELLOW );
	
	// setup init current place type, erase mode, tics, and state
	
	currentPlaceType = Color::RED;
	eraseMode = false;
	tics = 0;
	state = GameState::PLACING;

}


void Game::update() {
	// make sure game state is running
	
	if (state != GameState::RUNNING) return; 

		// Move Update 
	for ( auto& [p, dat] : battlefield ) {
		if ( dat.occupant ) {
			Position target = dat.occupant->targetPos(battlefield);
			if ( !battlefield[target].occupant ) {
				dat.occupant->movePos(target);
				battlefield[target].occupant = dat.occupant;
				dat.occupant = nullptr;
			}	
		}
	}
		
		// Kill Update
	for ( auto& [p, dat] : battlefield ) {
		Combatant* guy = dat.occupant;
		if ( guy ) {
			bool survived = guy->survive(battlefield);
			if (!survived) {
				dat.occupant = nullptr;
			}
		}
	}

		
		// increment tics, set win / lose conditions

	tics++;


		// Win: All enemies eliminated
	bool anyEnemiesLeft = false;
	for (const auto& [pos, data] : battlefield) {
	    if (data.occupant && !data.occupant->isFriendly()) {
		anyEnemiesLeft = true;
		break;
	    }
	}

		// Loss: All friendlies eliminated
	bool anyFriendliesLeft = false;
	for (const auto& [pos, data] : battlefield) {
	    if (data.occupant && data.occupant->isFriendly()) {
		anyFriendliesLeft = true;
		break;
	    }
	}

	if (!anyEnemiesLeft && anyFriendliesLeft) {
	    state = GameState::WON;
	} else if (!anyFriendliesLeft) {
	    state = GameState::LOST;
	}



}



bool Game::isOccupied(Position p) {
	// TODO: code me
	return false;
}


void Game::deleteCombatant(Combatant* combatant) {
	// TODO: code me
}

void Game::addCombatant(Position p, Color c) {
	if ( battlefield[p].occupant ) { // delete existing combatant in position
		deleteCombatant( battlefield[p].occupant );
	}

	if (c == Color::PURPLE) {
		battlefield[p].occupant = new Attack(p, c, -1);
	}

	if (c == Color::RED) {
		battlefield[p].occupant = new Attack(p, c, 1);
	}

	if (c == Color::BLUE) {
		battlefield[p].occupant = new Ranged(p, c, -1);
	}

	if (c == Color::ORANGE) {
		battlefield[p].occupant = new Ranged(p, c, 1);
	}

	if (c == Color::GREEN) {
		battlefield[p].occupant = new Intel(p, c, -1);
	}

	if (c == Color::YELLOW) {
		battlefield[p].occupant = new Intel(p, c, 1);
	}
}



void Game::render() {
	renderer->clearScreen();
	renderer->renderBattlefield( battlefield, battlefieldRect );

	renderer->renderUI(state == GameState::PLACING, 
			   eraseMode, 
			   currentPlaceType,
			   playButtonRect,
		           eraseButtonRect,
		           resetButtonRect,
			   typeButtonRects  );

	renderer->renderText("Play", playButtonRect.x + 10, playButtonRect.y, {0, 0, 0}, font);

	renderer->renderText("Reset", resetButtonRect.x + 10, resetButtonRect.y, {0, 0, 0}, font);

		// end state
	if (state == GameState::WON) {
		renderer->renderText("YOU WIN", 100, 100, {0, 150, 0}, font);
	} else if (state == GameState::LOST) {
		renderer->renderText("YOU LOSE", 100, 100, {150, 0, 0}, font);
	}

	renderer->presentScreen();

}

void Game::handleEvent(SDL_Event& e) {
		// simple pass event to event handler (if it exists)
	if (eventHandler != nullptr) {
		eventHandler->handleEvent(e);
	}
}

void Game::cleanUp() {
		// delete font
	if (font) {
	    TTF_CloseFont(font);
	    font = nullptr;
	}
	TTF_Quit();
	
		// delete audio
	Mix_CloseAudio();

	if (renderer) { 
		delete renderer;
		renderer = nullptr;
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
	SDL_Quit();
}


Game::~Game() {
	cleanUp();
}
