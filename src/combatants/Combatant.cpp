//
// Created by J. Blackburn - Mar 28 2025
//

#include "Combatant.h"
#include <random>

#include <iostream>
using namespace std;

Combatant::Combatant(Position p, Color c) : position(p), color(c) {}

Position Combatant::getPosition() {
	return position;
}
	

bool Combatant::move() {
	Traits ts = getTraits();
	// TODO: implement move logic
	
	cout << "Combatant is making a move!" << endl;
	return true;
}


bool Combatant::survive() {
	// TODO: implement survival logic
	
	cout << "Combatant is trying to survive!" << endl;
	return true;
}
