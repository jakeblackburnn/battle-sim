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

	Position targetPos(Battlefield& bf); // get the combatants desired next position

	bool survive(Battlefield& bf) const;  // Try to survive based on some 
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
				  

	MoveOption selectMove(Battlefield& bf, const Traits& traits);
		// movement selection depends on 
		// nearby enemies and allies
	int countNearbyEnemies(Battlefield& bf, int range) const;
	int countNearbyFriendlies(Battlefield& bf, int range) const;

		// stick move direction depends on direction
		// w/ greatest number of friendlies
	int countForwardFriendlies(Battlefield& bf, int range) const;
	int countBackFriendlies(Battlefield& bf, int range) const;
	int countLeftFriendlies(Battlefield& bf, int range) const;
	int countRightFriendlies(Battlefield& b, int range) const;

};

#endif
