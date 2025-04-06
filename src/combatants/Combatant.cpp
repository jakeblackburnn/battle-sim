
// Created by J. Blackburn - Mar 28 2025
//

#include "Combatant.h"

#include <random>
#include <iostream>
using namespace std;

MoveOption selectMove(const Traits& traits);

Combatant::Combatant(Position p, Color c, int o) : position(p), color(c), orientation(o) {
	if (orientation == -1) { friendly = false; }
}

Color    Combatant::getColor() { return color; }
Position Combatant::getPos() { return position; }
bool     Combatant::isFriendly() { return friendly; }

void Combatant::movePos(Position p) {
	position = p;
}

Position Combatant::targetPos() {

	Traits     ts           = getTraits();
	MoveOption selection    = selectMove(ts);
	Position   new_position = position; 

	switch (selection) {

		case MoveOption::H  :  break; 

		case MoveOption::F  : { 
			new_position = Position( position.x + ( 1 * orientation ),  
					         position.y                        );
			break;
		}

		case MoveOption::FR : { 
			new_position = Position( position.x + ( 1 * orientation ), 
					         position.y + ( 1 * orientation )  );
			break;
		}

		case MoveOption::FL : { 
			new_position = Position( position.x + ( 1 * orientation ), 
					         position.y - ( 1 * orientation )  );
			break;
		}

		case MoveOption::R  : { 
			new_position = Position( 
					 position.x, 
					 position.y + ( 1 * orientation ) 
				       );
			break;
		}

		case MoveOption::L  : { 
			new_position = Position( 
					 position.x, 
					 position.y - ( 1 * orientation ) 
				       );
			break;
		}

		case MoveOption::B  : { 
			new_position = Position( 
					 position.x - ( 1 * orientation ), 
					 position.y 
				       );
			break;
		}

		case MoveOption::BR : { 
			new_position = Position( 
					 position.x - ( 1 * orientation ), 
					 position.y + ( 1 * orientation ) 
				       );
			break;
		}

		case MoveOption::BL : { 
			new_position = Position( 
					 position.x - ( 1 * orientation ), 
					 position.y - ( 1 * orientation ) 
				       );
			break;
		}

		case MoveOption::S  : break; // TODO: implement stick logic
	}

	if ( new_position.x < 100 && new_position.x >= 0  &&
	     new_position.y < 100 && new_position.y >= 0     )
	{ return new_position; } 

	return position;

}


bool Combatant::survive(Battlefield& b) const {
	
	int enemies = 0;
	int enemyScore = 15;
	int range = 8;

	int xl = position.x - range < 0   ? 0   : position.x - range;
	int xr = position.x + range > 100 ? 100 : position.x + range;
	int yt = position.y - range < 0   ? 0   : position.y - range;
	int yb = position.y + range > 100 ? 100 : position.y + range;

	for ( int x = xl; x <= xr; x++ ) {
	for ( int y = yt; y <= yb; y++ ) {

		Position p = Position(x, y);
		Combatant* c = b[p].occupant;
		if ( c && friendly != c->isFriendly() ) enemies++;
	}
	}


	int killScore = 1 + enemies * enemyScore; // base killScore of 1


	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, killScore + 151); // 150 survival score
	int roll = dis(gen);

	if ( roll <= killScore ) return false; // must roll higher than kill score to survive

	return true;
}

	// random movement selection from move option probability distribution (traits)
MoveOption selectMove(const Traits& traits) { 
	
	int total = traits.f  +
		    traits.fl +
		    traits.fr +
		    traits.h  +
		    traits.l  +
		    traits.r  +
		    traits.b  +
		    traits.bl +
		    traits.br +
		    traits.s;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, total);
	int roll = dis(gen);

	int runningTotal = 0;

	if ((runningTotal += traits.fr) > roll) return MoveOption::FR;
	if ((runningTotal += traits.f)  > roll) return MoveOption::F;
	if ((runningTotal += traits.fl) > roll) return MoveOption::FL;
	if ((runningTotal += traits.r)  > roll) return MoveOption::R;
	if ((runningTotal += traits.h)  > roll) return MoveOption::H;
	if ((runningTotal += traits.l)  > roll) return MoveOption::L;
	if ((runningTotal += traits.br) > roll) return MoveOption::BR;
	if ((runningTotal += traits.b)  > roll) return MoveOption::B;
	if ((runningTotal += traits.bl) > roll) return MoveOption::BL;

	return MoveOption::S;
}
