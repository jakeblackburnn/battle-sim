
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


	playButtonRect =  { 415, 20, 60, 30 };
	resetButtonRect = { 515, 20, 75, 30 };

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
			deleteCombatant(dat.occupant);
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

	// setup init current place type, erase mode, tics, and state
	
	currentPlaceType = Color::RED;
	eraseMode = false;
	tics = 0;
	state = GameState::PLACING;

}


void Game::update() {
	
		// make sure game state is running
	if (state != GameState::RUNNING) return; 

		// calculate "center of mass" for both armies
	int friendlyX = 0; 
	int friendlyY = 0;
	int enemyX = 0;
	int enemyY = 0;

	for ( auto& [p, dat] : battlefield ) {
		if ( dat.occupant ) {
			if ( dat.occupant->isFriendly() == true ) {
				friendlyX += p.x;
				friendlyY += p.y;
			} else {
				enemyX += p.x;
				enemyY += p.y;
			}
		}
	}

	Position fcom = Position( friendlyX / friendlyCount, friendlyY / friendlyCount );
	Position ecom = Position( enemyX / enemyCount, enemyY / enemyCount );

	int dx = ecom.x - fcom.x;
	int dy = ecom.y - fcom.y;


		// Move Update 
	for ( auto& [p, dat] : battlefield ) {
		if ( dat.occupant ) {
			Position target = dat.occupant->targetPos(battlefield, dx, dy);
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
				deleteCombatant(guy);
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
	return battlefield[p].occupant != nullptr;
}


void Game::deleteCombatant(Combatant* combatant) {
	if (!combatant) return;

	Position pos = combatant->getPos();

	auto it = battlefield.find(pos);
	if (it != battlefield.end() && it->second.occupant == combatant) {
		if (combatant->isFriendly()) {
		    friendlyCount--;
		} else {
		    enemyCount--;
		}
		delete combatant;
		it->second.occupant = nullptr;
	}
}

void Game::addCombatant(Position p, Color c) {
	if ( battlefield[p].occupant ) { // if position occupied, dont add anything
		return;
	}

	if (c == Color::PURPLE) {
		battlefield[p].occupant = new Attack(p, c, -1);
		enemyCount++;
	}

	if (c == Color::RED) {
		battlefield[p].occupant = new Attack(p, c, 1);
		friendlyCount++;
	}

	if (c == Color::BLUE) {
		battlefield[p].occupant = new Ranged(p, c, -1);
		enemyCount++;
	}

	if (c == Color::ORANGE) {
		battlefield[p].occupant = new Ranged(p, c, 1);
		friendlyCount++;
	}

	if (c == Color::GREEN) {
		battlefield[p].occupant = new Intel(p, c, -1);
		enemyCount++;
	}

	if (c == Color::YELLOW) {
		battlefield[p].occupant = new Intel(p, c, 1);
		friendlyCount++;
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

	renderer->renderText("Combatant Selection", typeButtonRects[0].x, typeButtonRects[0].y - 40, {0,0,0}, font);

	renderer->renderText("Attack Unit", typeButtonRects[0].x + 40, typeButtonRects[0].y, {0,0,0}, font);
	renderer->renderText("Ranged Unit", typeButtonRects[1].x + 40, typeButtonRects[1].y, {0,0,0}, font);
	renderer->renderText("Intel Unit", typeButtonRects[2].x + 40, typeButtonRects[2].y, {0,0,0}, font);
	renderer->renderText("Erase", eraseButtonRect.x + 40, eraseButtonRect.y, {0,0,0}, font);

	renderer->renderText("Friendlies: " + std::to_string(friendlyCount), 20, 700, {0,0,0}, font);
	renderer->renderText("Enemies: " + std::to_string(enemyCount), 20, 730, {0,0,0}, font);
	renderer->renderText("Tics: " + std::to_string(tics), 20, 760, {0,0,0}, font);

		// end state
	if (state == GameState::WON) {
		renderer->renderText("YOU WIN", 450, 100, {0, 150, 0}, font);
	} else if (state == GameState::LOST) {
		renderer->renderText("YOU LOSE", 450, 100, {150, 0, 0}, font);
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
