//
// Created by J. Blackburn - Mar 27 2025
//

#ifndef COMBATANT_H
#define COMBATANT_H

enum class Color  { RED, ORANGE, YELLOW, PURPLE, BLUE, GREEN };
enum class MoveOption { F, FR, FL, L, H, R, B, BR, BL, S };

struct Position {
	int x;
	int y;
	Position(int px, int py) : x(px), y(py) {}
};

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
	// Color    getColor();
	Position getPosition();

	bool move();     // Make a decision on how to move (MoveOption), and then attempt that movement
			 // returns false if attempted move could not be completed

	bool survive();  // Try to survive based on some information about nearby enemies
			 // returns false if they didnt make it

protected: 
	Combatant(Position p, Color c);
	virtual const Traits& getTraits() const = 0; // get the static const traits struct 
						     // which is only defined for subclasses of abstract combatant

private:
	Position position;
	Color    color;
	int      orientation = 1; // assumed to be 1 for friendlies or -1 for enemies
};

#endif
