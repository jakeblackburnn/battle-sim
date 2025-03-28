// 
// Created by J. Blackburn - Mar 27 2025
//

#include "Attack.h"

const Traits Attack::traits = {
	1, // forward
	1, // forward right
	1, // forward left
	1, // left
	1, // hold position
	1, // right
	1, // back
	1, // back right
	1, // back left
	1  // stick to friendlies
}

Attack::Attack(Position p, Color c);
