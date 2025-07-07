
// Created by J. Blackburn - Mar 28 2025
//

#include "Combatant.h"

#include <random>
#include <iostream>
#include <algorithm>
using namespace std;

MoveOption selectMove(const Traits& traits);

Combatant::~Combatant() {}

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







Position Combatant::targetPos(Battlefield& bf, Position fcom, Position ecom) {


	int dx = ecom.x - fcom.x;
	int dy = ecom.y - fcom.y;

	Traits     ts           = getTraits();
	MoveOption selection    = selectMove(bf, ts, dx, dy);
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
			int x = position.x;
			int y = position.y;
			if (friendly) {
				if (position.x < fcom.x) {
					x += 1;
				} else {
					x -= 1;
				}
				if (position.y < fcom.y) {
					y += 1;
				} else {
					y -= 1;
				}
			} else {
				if (position.x < ecom.x) {
					x += 1;
				} else {
					x -= 1;
				}
				if (position.y < ecom.y) {
					y += 1;
				} else {
					y -= 1;
				}
			}
			new_position = Position(x, y);
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
MoveOption Combatant::selectMove(Battlefield& bf, const Traits& traits, int dx, int dy) { 

	int range = 8;
	int advantageMultiplier = 10;
	int disadvantageMultiplier = 10;
	int crowdednessMultiplier = 5;

	int enemies    = this->countNearbyEnemies(bf, range);
	int friendlies = this->countNearbyFriendlies(bf, range);
	int numbering    = max(0, friendlies - enemies); // measure of advantage
	int outnumbering = max(0, enemies - friendlies); // measure of disadvantage

	int advantage    = numbering    * advantageMultiplier;
	int disadvantage = outnumbering * disadvantageMultiplier;
	int crowdedness  = friendlies * crowdednessMultiplier;


	int advance = max(1, (traits.advance + advantage) - disadvantage);
	int retreat = max(1, (traits.retreat + disadvantage) - advantage);

	int af = (traits.flank + traits.advance) / 2;
	int rf = (traits.flank + traits.retreat) / 2;

	int advance_flank = max(1, af + ( (advantage - disadvantage) / 2 ) );
	int retreat_flank = max(1, rf + ( (disadvantage - advantage) / 2 ) );


	int s = traits.stick + max(0, traits.stick - crowdedness);
	int h = traits.hold  + max(0, traits.hold - abs(advantage - disadvantage));

	int f = 0;
	int fl = 0;
	int fr = 0;

	int l = 0;
	int r = 0;

	int b = 0;
	int bl = 0;
	int br = 0;

	if (abs(dx) > abs(dy)) {
			// advancing direction (dir of greatest com difference) is horizontal
		if (dx > 0) {
				// armies com are forward facing, forward orientation
			f = advance;
			fl = advance_flank;
			fr = advance_flank;

			l = traits.flank;
			r = traits.flank;

			b = retreat;
			bl = retreat_flank;
			br = retreat_flank;

		} else {
				// armies com are turned around, backward orientation
			b = advance;
			bl = advance_flank;
			br = advance_flank;

			l = traits.flank;
			r = traits.flank;

			f = retreat;
			fl = retreat_flank;
			fr = retreat_flank;
		}
	} else {
			// advancing direction is vertical
		if (dy > 0) {
				// armies com are to the right each other, rightward orientation
			r = advance;
			fr = advance_flank;
			br = advance_flank;

			f = traits.flank;
			b = traits.flank;

			l = retreat;
			fl = retreat_flank;
			bl = retreat_flank;
		} else {
				// armies com are to the left of each other, leftward orientation
			l = advance;
			fl = advance_flank;
			bl = advance_flank;

			f = traits.flank;
			b = traits.flank;

			r = retreat;
			fr = retreat_flank;
			br = retreat_flank;
		}
	}

	int total = f  +
		    fl +
		    fr +
		    l  +
		    h  +
		    r  +
		    b  +
		    bl +
		    br +
		    s;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(1, total);
	int roll = dis(gen);

	int runningTotal = 0;

	if ((runningTotal += fr) > roll) return MoveOption::FR;
	if ((runningTotal += f)  > roll) return MoveOption::F;
	if ((runningTotal += fl) > roll) return MoveOption::FL;
	if ((runningTotal += r)  > roll) return MoveOption::R;
	if ((runningTotal += h)  > roll) return MoveOption::H;
	if ((runningTotal += l)  > roll) return MoveOption::L;
	if ((runningTotal += br) > roll) return MoveOption::BR;
	if ((runningTotal += b)  > roll) return MoveOption::B;
	if ((runningTotal += bl) > roll) return MoveOption::BL;

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

