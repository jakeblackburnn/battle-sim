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
	       currentPlaceType(TroopType::RED),
	       eraseMode(false),
	       clickSound(nullptr),
	       battleMusic(nullptr)
{


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
		// Create Renderer
	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!sdlRenderer) {
		std::cerr << "Renderer Creation Failed." << SDL_GetError() << std::endl;
		return false;
	}
	renderer = new Renderer(sdlRenderer, 350, 250);

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

	// clear troop vectors
	red.clear();
	orange.clear();
	yellow.clear();
	green.clear();
	blue.clear();
	purple.clear();
	
	// Place Enemy troops

	purple.push_back( Troop(50, 20, TroopType::PURPLE) );
	blue.push_back( Troop( 50, 40, TroopType::BLUE) );
	green.push_back( Troop( 50, 60, TroopType::GREEN ) );
	
	// setup init current place type, erase mode, tics, and state
	
	currentPlaceType = TroopType::RED;
	eraseMode = false;
	tics = 0;
	state = GameState::PLACING;

}


void Game::update() {
	// make sure game state is running
	
	if (state != GameState::RUNNING) return; 
		// Move Update 
	move();

		// Kill Update
	kill();
		// increment tics, set win / lose conditions
	tics++;
	if ( red.empty() && orange.empty() && yellow.empty() ) state = GameState::LOST;
	if ( purple.empty() && blue.empty() && green.empty() ) state = GameState::WON;
}

void Game::move() {
	
	for (auto iter = red.begin(); iter != red.end(); iter++) {
		int x = iter->x;
		int y = iter->y;
			// red move choices
		int fr = 50;
	        int f = 100;
		int fl = 50;
		int r = 25;
		int h = 50;
		int l = 25;
		int br = 10;
		int b  = 25;
		int bl = 10;
		int s = 100;
			// select move
		MoveOption move = selectMove(fr, f, fl, r, h, l, br, b, bl, s);
			
			// Make Moves
		if (move == MoveOption::H) { continue; } // if holding position, do nothing
		if (move == MoveOption::S) {
			int frontFriendlys = countFrontFriendlys(x, y);
			int leftFriendlys  = countLeftFriendlys(x, y);
			int rightFriendlys = countRightFriendlys(x, y);
			int backFriendlys  = countBackFriendlys(x, y);

			if (frontFriendlys >= leftFriendlys && frontFriendlys >= rightFriendlys && frontFriendlys >= backFriendlys) {
				if (!isOccupied(x + 1, y)) iter->x++; // move forward
				continue;
			}
			if (leftFriendlys >= frontFriendlys && leftFriendlys >= rightFriendlys && leftFriendlys >= backFriendlys) {
				if (!isOccupied(x, y + 1)) iter->y++; // move left
				continue;
			}
			if (rightFriendlys >= frontFriendlys && rightFriendlys >= leftFriendlys && rightFriendlys >= backFriendlys) {
				if (!isOccupied(x, y - 1)) iter->y--; // move right
				continue;
			}
			if (backFriendlys >= leftFriendlys && backFriendlys >= rightFriendlys && backFriendlys >= frontFriendlys) {
				if (!isOccupied(x - 1, y)) iter->x--; // move backward
				continue;
			}

			continue; // make sure loop continues even if something breaks
		}

		if (move == MoveOption::F) {
			if (!isOccupied(x + 1, y)) iter->x++;
			continue;
		}
		if (move == MoveOption::FR) {
			if (!isOccupied(x + 1, y - 1)) {
				iter->x++;
				iter->y--;
			}
			continue;
		}
		if (move == MoveOption::FL) {
			if (!isOccupied(x + 1, y + 1)) {
				iter->x++;
				iter->y++;
			}
			continue;
		}
		if (move == MoveOption::R) {
			if (!isOccupied(x, y - 1)) iter->y--;
			continue;
		}
		if (move == MoveOption::L) {
			if (!isOccupied(x, y + 1)) iter->y++;
			continue;
		}
		if (move == MoveOption::B) {
			if (!isOccupied(x - 1, y)) iter->x--;
			continue;
		}
		if (move == MoveOption::BR) {
			if (!isOccupied(x - 1, y - 1)) {
				iter->x--;
				iter->y--;
			}
			continue;
		}
		if (move == MoveOption::BL) {
			if (!isOccupied(x - 1, y + 1)) {
				iter->x--;
				iter->y++;
			}
			continue;
		}



	} // end move reds
	  
	for (auto iter = orange.begin(); iter != orange.end(); iter++) {
		int x = iter->x;
		int y = iter->y;
			// red move choices
		int fr = 25;
	        int f = 200;
		int fl = 25;
		int r = 100;
		int h = 400;
		int l = 100;
		int br = 25;
		int b  = 100;
		int bl = 25;
		int s = 50;
			// select move
		MoveOption move = selectMove(fr, f, fl, r, h, l, br, b, bl, s);

			// Make Moves
		if (move == MoveOption::H) { continue; } // if holding position, do nothing
		if (move == MoveOption::S) {
			int frontFriendlys = countFrontFriendlys(x, y);
			int leftFriendlys  = countLeftFriendlys(x, y);
			int rightFriendlys = countRightFriendlys(x, y);
			int backFriendlys  = countBackFriendlys(x, y);

			if (frontFriendlys >= leftFriendlys && frontFriendlys >= rightFriendlys && frontFriendlys >= backFriendlys) {
				if (!isOccupied(x + 1, y)) iter->x++; // move forward
				continue;
			}
			if (leftFriendlys >= frontFriendlys && leftFriendlys >= rightFriendlys && leftFriendlys >= backFriendlys) {
				if (!isOccupied(x, y + 1)) iter->y++; // move left
				continue;
			}
			if (rightFriendlys >= frontFriendlys && rightFriendlys >= leftFriendlys && rightFriendlys >= backFriendlys) {
				if (!isOccupied(x, y - 1)) iter->y--; // move right
				continue;
			}
			if (backFriendlys >= leftFriendlys && backFriendlys >= rightFriendlys && backFriendlys >= frontFriendlys) {
				if (!isOccupied(x - 1, y)) iter->x--; // move backward
				continue;
			}

			continue; // make sure loop continues even if something breaks
		}

		if (move == MoveOption::F) {
			if (!isOccupied(x + 1, y)) iter->x++;
			continue;
		}
		if (move == MoveOption::FR) {
			if (!isOccupied(x + 1, y - 1)) {
				iter->x++;
				iter->y--;
			}
			continue;
		}
		if (move == MoveOption::FL) {
			if (!isOccupied(x + 1, y + 1)) {
				iter->x++;
				iter->y++;
			}
			continue;
		}
		if (move == MoveOption::R) {
			if (!isOccupied(x, y - 1)) iter->y--;
			continue;
		}
		if (move == MoveOption::L) {
			if (!isOccupied(x, y + 1)) iter->y++;
			continue;
		}
		if (move == MoveOption::B) {
			if (!isOccupied(x - 1, y)) iter->x--;
			continue;
		}
		if (move == MoveOption::BR) {
			if (!isOccupied(x - 1, y - 1)) {
				iter->x--;
				iter->y--;
			}
			continue;
		}
		if (move == MoveOption::BL) {
			if (!isOccupied(x - 1, y + 1)) {
				iter->x--;
				iter->y++;
			}
			continue;
		}


	} // end move oranges
	
	for (auto iter = yellow.begin(); iter != yellow.end(); iter++) {
		int x = iter->x;
		int y = iter->y;
			// yellow move choices
		int fr = 50;
	        int f = 25;
		int fl = 50;
		int r = 25;
		int h = 50;
		int l = 25;
		int br = 50;
		int b  = 25;
		int bl = 50;
		int s = 500;
			// select move
		MoveOption move = selectMove(fr, f, fl, r, h, l, br, b, bl, s);
			// Make Moves
		if (move == MoveOption::H) { continue; } // if holding position, do nothing
		if (move == MoveOption::S) {
			int frontFriendlys = countFrontFriendlys(x, y);
			int leftFriendlys  = countLeftFriendlys(x, y);
			int rightFriendlys = countRightFriendlys(x, y);
			int backFriendlys  = countBackFriendlys(x, y);

			if (frontFriendlys >= leftFriendlys && frontFriendlys >= rightFriendlys && frontFriendlys >= backFriendlys) {
				if (!isOccupied(x + 1, y)) iter->x++; // move forward
				continue;
			}
			if (leftFriendlys >= frontFriendlys && leftFriendlys >= rightFriendlys && leftFriendlys >= backFriendlys) {
				if (!isOccupied(x, y + 1)) iter->y++; // move left
				continue;
			}
			if (rightFriendlys >= frontFriendlys && rightFriendlys >= leftFriendlys && rightFriendlys >= backFriendlys) {
				if (!isOccupied(x, y - 1)) iter->y--; // move right
				continue;
			}
			if (backFriendlys >= leftFriendlys && backFriendlys >= rightFriendlys && backFriendlys >= frontFriendlys) {
				if (!isOccupied(x - 1, y)) iter->x--; // move backward
				continue;
			}

			continue; // make sure loop continues even if something breaks
		}

		if (move == MoveOption::F) {
			if (!isOccupied(x + 1, y)) iter->x++;
			continue;
		}
		if (move == MoveOption::FR) {
			if (!isOccupied(x + 1, y - 1)) {
				iter->x++;
				iter->y--;
			}
			continue;
		}
		if (move == MoveOption::FL) {
			if (!isOccupied(x + 1, y + 1)) {
				iter->x++;
				iter->y++;
			}
			continue;
		}
		if (move == MoveOption::R) {
			if (!isOccupied(x, y - 1)) iter->y--;
			continue;
		}
		if (move == MoveOption::L) {
			if (!isOccupied(x, y + 1)) iter->y++;
			continue;
		}
		if (move == MoveOption::B) {
			if (!isOccupied(x - 1, y)) iter->x--;
			continue;
		}
		if (move == MoveOption::BR) {
			if (!isOccupied(x - 1, y - 1)) {
				iter->x--;
				iter->y--;
			}
			continue;
		}
		if (move == MoveOption::BL) {
			if (!isOccupied(x - 1, y + 1)) {
				iter->x--;
				iter->y++;
			}
			continue;
		}



	} // end move yellows
	
}


MoveOption Game::selectMove(int fr, int f, int fl, int r, int h, int l, int br, int b, int bl, int s) {

	int totalScore = fr + f + fl + r + h + l + br + b + bl + s;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, totalScore - 1);

	int random = dis(gen);

	int runningTotal = 0;
	
	if (random < fr) return MoveOption::FR; 
	runningTotal += fr;
	if (random < runningTotal + f) return MoveOption::F; 
	runningTotal += f;
	if (random < runningTotal + fl) return MoveOption::FL; 
	runningTotal += fl;
	if (random < runningTotal + r) return MoveOption::R; 
	runningTotal += r;
	if (random < runningTotal + h) return MoveOption::H; 
	runningTotal += h;
	if (random < runningTotal + l) return MoveOption::L; 
	runningTotal += l;
	if (random < runningTotal + br) return MoveOption::BR; 
	runningTotal += br;
	if (random < runningTotal + b) return MoveOption::B; 
	runningTotal += b;
	if (random < runningTotal + bl) return MoveOption::BL; 

	return MoveOption::S;
}


bool Game::isOccupied(int x, int y) {

	// if out of bounds return false by default
	
	if (x > 100 || y > 100) {
		return true;
	}

	for (auto iter = red.begin(); iter != red.end(); iter++) {
		if (iter->x == x && iter->y == y) {
			return true;
		}
	}
	for (auto iter = orange.begin(); iter != orange.end(); iter++) {
		if (iter->x == x && iter->y == y) {
			return true;
		}
	}
	for (auto iter = yellow.begin(); iter != yellow.end(); iter++) {
		if (iter->x == x && iter->y == y) {
			return true;
		}
	}
	for (auto iter = purple.begin(); iter != purple.end(); iter++) {
		if (iter->x == x && iter->y == y) {
			return true;
		}
	}
	for (auto iter = blue.begin(); iter != blue.end(); iter++) {
		if (iter->x == x && iter->y == y) {
			return true;
		}
	}
	for (auto iter = green.begin(); iter != green.end(); iter++) {
		if (iter->x == x && iter->y == y) {
			return true;
		}
	}

	return false;
}







void Game::kill() {
	std::vector<Troop*> toKill;


	// pick dead troops
	
	for (auto& t : red) {
		int l = 100;
		int d = 1;
		int closePurples = countClosePurples(t.x, t.y);
		int midPurples   = countMidPurples(t.x, t.y);
		int midBlues     = countMidBlues(t.x, t.y);
		int longBlues    = countLongBlues(t.x, t.y);
		int midGreens    = countMidGreens(t.x, t.y);


		int greenEffect = (closePurples + midPurples + midBlues + longBlues) * midGreens;

		int more_d = ( closePurples * 25 + 
			       midPurples * 10 + 
			       midBlues * 10 +
			       longBlues * 25 ) + greenEffect;

		d = d + more_d;

		bool kill = selectKill(l, d);

		if (kill) toKill.push_back(&t);
	}

	for (auto& t : orange) {
		int l = 100;
		int d = 1;
		int closePurples = countClosePurples(t.x, t.y);
		int midPurples   = countMidPurples(t.x, t.y);
		int midBlues     = countMidBlues(t.x, t.y);
		int longBlues    = countLongBlues(t.x, t.y);
		int midGreens    = countMidGreens(t.x, t.y);


		int greenEffect = (closePurples + midPurples + midBlues + longBlues) * midGreens;

		int more_d = ( closePurples * 25 + 
			       midPurples * 10 + 
			       midBlues * 10 +
			       longBlues * 25 ) + greenEffect;

		d = d + more_d;

		bool kill = selectKill(l, d);

		if (kill) toKill.push_back(&t);
	}

	for (auto& t : yellow) {
		int l = 100;
		int d = 1;
		int closePurples = countClosePurples(t.x, t.y);
		int midPurples   = countMidPurples(t.x, t.y);
		int midBlues     = countMidBlues(t.x, t.y);
		int longBlues    = countLongBlues(t.x, t.y);
		int midGreens    = countMidGreens(t.x, t.y);


		int greenEffect = (closePurples + midPurples + midBlues + longBlues) * midGreens;

		int more_d = ( closePurples * 25 + 
			       midPurples * 10 + 
			       midBlues * 10 +
			       longBlues * 25 ) + greenEffect;

		d = d + more_d;

		bool kill = selectKill(l, d);

		if (kill) toKill.push_back(&t);
	}

	for (auto& t : purple) {
		int l = 100;
		int d = 1;
		int closeReds = countCloseReds(t.x, t.y);
		int midReds   = countMidReds(t.x, t.y);
		int midOranges     = countMidOranges(t.x, t.y);
		int longOranges    = countLongOranges(t.x, t.y);
		int midYellows    = countMidYellows(t.x, t.y);


		int yellowEffect = (closeReds + midReds + midOranges + longOranges) * midYellows;

		int more_d = ( closeReds * 25 + 
			       midReds * 10 + 
			       midOranges * 10 +
			       longOranges * 25 ) + yellowEffect;

		d = d + more_d;

		bool kill = selectKill(l, d);

		if (kill) toKill.push_back(&t);
	}

	for (auto& t : blue) {
		int l = 100;
		int d = 1;
		int closeReds = countCloseReds(t.x, t.y);
		int midReds   = countMidReds(t.x, t.y);
		int midOranges     = countMidOranges(t.x, t.y);
		int longOranges    = countLongOranges(t.x, t.y);
		int midYellows    = countMidYellows(t.x, t.y);


		int yellowEffect = (closeReds + midReds + midOranges + longOranges) * midYellows;

		int more_d = ( closeReds * 25 + 
			       midReds * 10 + 
			       midOranges * 10 +
			       longOranges * 25 ) + yellowEffect;

		d = d + more_d;

		bool kill = selectKill(l, d);

		if (kill) toKill.push_back(&t);
	}

	for (auto& t : green) {
		int l = 100;
		int d = 1;
		int closeReds = countCloseReds(t.x, t.y);
		int midReds   = countMidReds(t.x, t.y);
		int midOranges     = countMidOranges(t.x, t.y);
		int longOranges    = countLongOranges(t.x, t.y);
		int midYellows    = countMidYellows(t.x, t.y);


		int yellowEffect = (closeReds + midReds + midOranges + longOranges) * midYellows;

		int more_d = ( closeReds * 25 + 
			       midReds * 10 + 
			       midOranges * 10 +
			       longOranges * 25 ) + yellowEffect;

		d = d + more_d;

		bool kill = selectKill(l, d);

		if (kill) toKill.push_back(&t);
	}

	
		// remove dead troops
	for (Troop* t : toKill) {
		if (t->type == TroopType::RED) {
			deleteTroop(red, t);
		}
		if (t->type == TroopType::ORANGE) {
			deleteTroop(orange, t);
		}
		if (t->type == TroopType::YELLOW) {
			deleteTroop(yellow, t);
		}
		if (t->type == TroopType::PURPLE) {
			deleteTroop(purple, t);
		}
		if (t->type == TroopType::BLUE) {
			deleteTroop(blue, t);
		}
		if (t->type == TroopType::GREEN) {
			deleteTroop(green, t);
		}
	}
}


bool Game::selectKill(int l, int d) {

	int totalScore = l + d;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, totalScore - 1);

	int random = dis(gen);

	if (random > l) {
		return true;
	} else {
		return false;
	}
}

void Game::deleteTroop(TroopVector troops, Troop* t) {
	for (auto iter = troops.begin(); iter != troops.end(); iter++) {
		if (iter->x == t->x && iter->y == t->y) { 
			troops.erase(iter);
			return;
		}
	}
}



void Game::render() {

	renderer->clearScreen();

	renderer->renderBattlefield(red, orange, yellow, purple, blue, green);

	renderer->renderUI(state == GameState::PLACING, 
			   eraseMode, 
			   currentPlaceType,
			   playButtonRect,
			   nextButtonRect,
		           eraseButtonRect,
			   typeButtonRects );

	if (state == GameState::WON || state == GameState::LOST) {
		renderer->renderGameOverMessage(state == GameState::WON);
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



int Game::countFrontFriendlys(int x, int y) {
	int sum = 0;

	for (auto& fr : red) {
		if (fr.x > x) sum++;
	}
	for (auto& fr : yellow) {
		if (fr.x > x) sum++;
	}
	for (auto& fr : orange) {
		if (fr.x > x) sum++;
	}

	return sum;
}

int Game::countBackFriendlys(int x, int y) {
	int sum = 0;

	for (auto& fr : red) {
		if (fr.x < x) sum++;
	}
	for (auto& fr : yellow) {
		if (fr.x < x) sum++;
	}
	for (auto& fr : orange) {
		if (fr.x < x) sum++;
	}

	return sum;
}

int Game::countLeftFriendlys(int x, int y) {
	int sum = 0;

	for (auto& fr : red) {
		if (fr.y > y) sum++;
	}
	for (auto& fr : yellow) {
		if (fr.y > y) sum++;
	}
	for (auto& fr : orange) {
		if (fr.y > y) sum++;
	}

	return sum;
}

int Game::countRightFriendlys(int x, int y) {
	int sum = 0;

	for (auto& fr : red) {
		if (fr.y < y) sum++;
	}
	for (auto& fr : yellow) {
		if (fr.y < y) sum++;
	}
	for (auto& fr : orange) {
		if (fr.y < y) sum++;
	}

	return sum;
}



int Game::countClosePurples(int x, int y) {
	int sum = 0;

	for (auto& p : purple) {
		if (abs( x - p.x ) <= 4 && abs( y - p.y ) <= 4) sum++;
	}
	return sum;
}

int Game::countMidPurples(int x, int y) {
	int sum = 0;

	for (auto& p : purple) {
		if (abs( x - p.x ) <= 8 && abs( y - p.y ) <= 8 ) {
			if (abs(x - p.x) > 4 && abs(y - p.y) > 4) sum++;
		}
	}
	return sum;
}

int Game::countMidBlues(int x, int y) {
	int sum = 0;

	for (auto& b : blue) {
		if (abs( x - b.x ) <= 8 && abs( y - b.y ) <= 8 ) {
			if (abs(x - b.x) > 4 && abs(y - b.y) > 4) sum++;
		}
	}
	return sum;
}

int Game::countLongBlues(int x, int y) {
	int sum = 0;

	for (auto& b : blue) {
		if (abs( x - b.x ) <= 16 && abs( y - b.y ) <= 16 ) {
			if (abs(x - b.x) > 8 && abs(y - b.y) > 8) sum++;
		}
	}
	return sum;
}

int Game::countMidGreens(int x, int y) {
	int sum = 0;

	for (auto& g : green) {
		if (abs( x - g.x ) <= 8 && abs( y - g.y ) <= 8 ) {
			if (abs(x - g.x) > 4 && abs(y - g.y) > 4) sum++;
		}
	}
	return sum;
}


int Game::countCloseReds(int x, int y) {
	int sum = 0;

	for (auto& r : red) {
		if (abs( x - r.x ) <= 4 && abs( y - r.y ) <= 4) sum++;
	}
	return sum;
}

int Game::countMidReds(int x, int y) {
	int sum = 0;

	for (auto& r : red) {
		if (abs( x - r.x ) <= 8 && abs( y - r.y ) <= 8 ) {
			if (abs(x - r.x) > 4 && abs(y - r.y) > 4) sum++;
		}
	}
	return sum;
}

int Game::countMidOranges(int x, int y) {
	int sum = 0;

	for (auto& o : orange) {
		if (abs( x - o.x ) <= 8 && abs( y - o.y ) <= 8 ) {
			if (abs(x - o.x) > 4 && abs(y - o.y) > 4) sum++;
		}
	}
	return sum;
}

int Game::countLongOranges(int x, int y) {
	int sum = 0;

	for (auto& o : orange) {
		if (abs( x - o.x ) <= 16 && abs( y - o.y ) <= 16 ) {
			if (abs(x - o.x) > 8 && abs(y - o.y) > 8) sum++;
		}
	}
	return sum;
}

int Game::countMidYellows(int x, int y) {
	int sum = 0;

	for (auto& yl : yellow) {
		if (abs( x - yl.x ) <= 8 && abs( y - yl.y ) <= 8 ) {
			if (abs(x - yl.x) > 4 && abs(y - yl.y) > 4) sum++;
		}
	}
	return sum;
}
