#ifndef TROOP_H
#define TROOP_H

enum class TroopType { RED, ORANGE, YELLOW, PURPLE, BLUE, GREEN };

struct Troop {
	int x;
	int y;
	TroopType type;
	
	Troop(int px, int py, TroopType t) : x(px), y(py), type(t) {}

#endif 
