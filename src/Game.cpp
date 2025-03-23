//
// Created by J. Blackburn - Mar 22 2025
//

#include "Game.h"
#include "EventHandler.h"
#include <iostream>


Game::Game() : window(nullptr), 
	       renderer(nullptr), 
	       eventHandler(nullptr),
	       state(GameState::PLACING), 
	       running(false), 
	       currentLevel(1), 
	       tics(0), 
	       maxTics(1000),
	       budget(20), 
	       currentPlaceType(TroopType::RED),
	       eraseMode(false),
	       clickSound(nullptr),
	       battleMusic(nullptr)
{

	eventHandler = new EventHandler(this);

	playButtonRect = { 270, 10, 60, 30 };
	nextButtonRect = { 250, 10, 100, 30 };

	for (int i = 0; i < 3; i++) {
		typeButtonRects[i] = { 10 + i*40, 10, 30, 30 };
	}

	eraseButtonRect = { 10 + 3*40, 10, 30, 30 };
}


bool Game::init() {

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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cerr << "Renderer Creation Failed." << SDL_GetError() << std::endl;
		return false;
	}

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





void Game::setupLevel( int level ) {

	currentLevel = level;

	// clear troop vectors
	
	// set budget based on level
	
	// Place Enemy troops
	
	// setup init current place type, erase mode, tics, and state

}


void Game::update() {
	// make sure game state is running
	//
	// Move Update 
	//
	// Kill Update
	//
	// increment tics
}


void Game::render() {
	// renderer logic
}

void Game::handleEvent(SDL_Event& e) {
		// simple pass event to event handler (if it exists)
	if (eventHandler != nullptr) {
		eventHandler->handleEvent(e);
	}
}

void Game::cleanUp() {
	// delete audio
	Mix_CloseAudio();

	if (renderer) SDL_DestroyRenderer(renderer);
	if (window)   SDL_DestroyWindow(window);
	SDL_Quit();
}


Game::~Game() {
	cleanUp();
}

