
// Created by J. Blackburn - Mar 28 2025
//

#include "Combatant.h"

#include <random>
#include <iostream>
#include <algorithm>
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

bool Combatant::survive(Battlefield& bf) const {
	
	int enemyScore = 25;
	int range = 8;
	int surviveScore = 200;

	int enemies = this->countNearbyEnemies(bf, range);


	int killScore = 1 + enemies * enemyScore; // base killScore of 1


	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, killScore + surviveScore + 1); 
	int roll = dis(gen);

	if ( roll <= killScore ) return false; // must roll higher than kill score to survive

	return true;
}







Position Combatant::targetPos(Battlefield& bf) {

	Traits     ts           = getTraits();
	MoveOption selection    = selectMove(bf, ts);
	Position   new_position = position; 

	int range = 8;

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
		case MoveOption::S  : {
			int f = this->countForwardFriendlies(bf, range);
			int b = this->countBackFriendlies(bf, range);
			int r = this->countRightFriendlies(bf, range);
			int l = this->countLeftFriendlies(bf, range);

			int high = max({f, b, r, l});

			if (f == high) {
				new_position = Position( position.x + ( 1 * orientation ),  
							 position.y                        );
				break;
			}
			if (b == high) {
				new_position = Position( 
						 position.x - ( 1 * orientation ), 
						 position.y 
					       );
				break;
			}
			if (r == high) {
				new_position = Position( 
						 position.x, 
						 position.y + ( 1 * orientation ) 
					       );
				break;
			}
			if (l == high) {
				new_position = Position( 
						 position.x, 
						 position.y - ( 1 * orientation ) 
					       );
				break;
			}
	        }
	}

	if ( new_position.x < 100 && new_position.x >= 0  &&
	     new_position.y < 100 && new_position.y >= 0     )
	{ return new_position; } 

	return position;
}

	// random movement selection from move option probability distribution (traits)
	// distribution changes based on num of nearby enemies / friendlies
	// more friendlies => less likely to stick, more likely to advance
	// enemies - friendlies (outnumbered)  => less likely to advance,
	// less likely to retreat
MoveOption Combatant::selectMove(Battlefield& bf, const Traits& traits) { 

	int range = 8;
	int advantageMultiplier = 5;
	int disadvantageMultiplier = 10;
	int crowdednessMultiplier = 10;

	int enemies    = this->countNearbyEnemies(bf, range);
	int friendlies = this->countNearbyFriendlies(bf, range);

	int numbering    = max(0, friendlies - enemies); // measure of advantage
	int outnumbering = max(0, enemies - friendlies); // measure of disadvantage

	int advantage    = numbering    * advantageMultiplier;
	int disadvantage = outnumbering * disadvantageMultiplier;

	int f  = max(1, (traits.f + advantage) - disadvantage);
	int b  = max(1, (traits.b + disadvantage) - advantage);

	int fl = traits.fl;
	int fr = traits.fr;

	int h  = traits.h;
	int l  = traits.l;
	int r  = traits.r;

	int bl = traits.bl;
	int br = traits.br;

	int crowdedness = friendlies * crowdednessMultiplier;

	int s  = max(1, traits.s - crowdedness);
	
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




// Counting methods


int Combatant::countNearbyEnemies(Battlefield& bf, int range) const {

	int enemies = 0;

	int xl = position.x - range < 0   ? 0   : position.x - range;
	int xr = position.x + range > 100 ? 100 : position.x + range;
	int yt = position.y - range < 0   ? 0   : position.y - range;
	int yb = position.y + range > 100 ? 100 : position.y + range;

	for ( int x = xl; x <= xr; x++ ) {
	for ( int y = yt; y <= yb; y++ ) {

		Position p = Position(x, y);
		Combatant* c = bf[p].occupant;

		if ( c ) {
			if ( friendly != c->isFriendly() ) enemies++;
		}

	}
	}

	return enemies;
}

int Combatant::countNearbyFriendlies(Battlefield& bf, int range) const {

	int friendlies = 0;

	int xl = position.x - range < 0   ? 0   : position.x - range;
	int xr = position.x + range > 100 ? 100 : position.x + range;
	int yt = position.y - range < 0   ? 0   : position.y - range;
	int yb = position.y + range > 100 ? 100 : position.y + range;

	for ( int x = xl; x <= xr; x++ ) {
	for ( int y = yt; y <= yb; y++ ) {

		Position p = Position(x, y);
		Combatant* c = bf[p].occupant;

		if ( c ) {
			if ( friendly == c->isFriendly() ) friendlies++;
		}
	}
	}

	return friendlies;
}


int Combatant::countForwardFriendlies(Battlefield& bf, int range) const {

	int friendlies = 0;

	int xl = position.x - range < 0   ? 0   : position.x - range;
	int xr = position.x + range > 100 ? 100 : position.x + range;
	int yt = position.y - range < 0   ? 0   : position.y - range;
	int yb = position.y + range > 100 ? 100 : position.y + range;

	if (friendly) {
		xl = position.x;
	} else {
		xr = position.x;
	}


	for ( int x = xl; x <= xr; x++ ) {
	for ( int y = yt; y <= yb; y++ ) {

		Position p = Position(x, y);
		Combatant* c = bf[p].occupant;
		if ( c ) {
			if (friendly == c->isFriendly() ) friendlies++;
		}
	}
	}

	return friendlies;
}

int Combatant::countBackFriendlies(Battlefield& bf, int range) const {

	int friendlies = 0;

	int xl = position.x - range < 0   ? 0   : position.x - range;
	int xr = position.x + range > 100 ? 100 : position.x + range;
	int yt = position.y - range < 0   ? 0   : position.y - range;
	int yb = position.y + range > 100 ? 100 : position.y + range;

	if (friendly) {
		xr = position.x;
	} else {
		xl = position.x;
	}

	for ( int x = xl; x <= xr; x++ ) {
	for ( int y = yt; y <= yb; y++ ) {

		Position p = Position(x, y);
		Combatant* c = bf[p].occupant;
		if ( c ) {
			if (friendly == c->isFriendly() ) friendlies++;
		}
	}
	}

	return friendlies;
}

int Combatant::countLeftFriendlies(Battlefield& bf, int range) const {

	int friendlies = 0;

	int xl = position.x - range < 0   ? 0   : position.x - range;
	int xr = position.x + range > 100 ? 100 : position.x + range;
	int yt = position.y - range < 0   ? 0   : position.y - range;
	int yb = position.y + range > 100 ? 100 : position.y + range;

	if (friendly) {
		yb = position.y;
	} else {
		yt = position.y;
	}

	for ( int x = xl; x <= xr; x++ ) {
	for ( int y = yt; y <= yb; y++ ) {

		Position p = Position(x, y);
		Combatant* c = bf[p].occupant;
		if ( c ) {
			if (friendly == c->isFriendly() ) friendlies++;
		}
	}
	}

	return friendlies;
}


int Combatant::countRightFriendlies(Battlefield& bf, int range) const {

	int friendlies = 0;

	int xl = position.x - range < 0   ? 0   : position.x - range;
	int xr = position.x + range > 100 ? 100 : position.x + range;
	int yt = position.y - range < 0   ? 0   : position.y - range;
	int yb = position.y + range > 100 ? 100 : position.y + range;

	if (friendly) {
		yt = position.y;
	} else {
		yb = position.y;
	}

	for ( int x = xl; x <= xr; x++ ) {
	for ( int y = yt; y <= yb; y++ ) {

		Position p = Position(x, y);
		Combatant* c = bf[p].occupant;
		if ( c ) {
			if (friendly == c->isFriendly() ) friendlies++;
		}
	}
	}

	return friendlies;
}

