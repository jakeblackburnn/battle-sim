//
// Created by J. Blackburn - Mar 27 2025
//

#ifndef COMBATANT_H
#define COMBATANT_H

#include "battlefield.h"

enum class Color  { RED, ORANGE, YELLOW, PURPLE, BLUE, GREEN };
enum class MoveOption { F, FR, FL, L, H, R, B, BR, BL, S };

struct Traits {    // Tendencies to make moves 
	int f;     // stored as integer "scores" which partition the space of possible moves;
	int fr;    // moves can be selected with a random number in the range of the total score
	int fl;
	int l;
	int h;
	int r;
	int b;
	int br;
	int bl;
	int s;
};

// Abstract Combatant
class Combatant {
public: 
	Color    getColor();
	Position getPos();
	void     movePos(Position p);
	bool     isFriendly();

	Position targetPos(); // get the combatants desired next position

	bool survive(Battlefield& b) const;  // Try to survive based on some 
					     // information about nearby enemies
				             // returns false if they didnt make it

protected: 
	Combatant(Position p, Color c, int o);
	virtual const Traits& getTraits() const = 0; // get the static const traits struct 
						     // which is only defined for subclasses of abstract combatant

private:
	Position position;
	Color    color;
	bool     friendly = true;
	int      orientation = 1; // assumed to be 1 for friendlies or -1 for enemies
};

#endif
