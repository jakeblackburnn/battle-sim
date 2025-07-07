//
// Created by J. Blackburn - Apr 5 2025
//

#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <vector>
#include <unordered_map>
using namespace std;

class Combatant; // forward declaration of Combatant class

struct Position {
	int x;
	int y;
	Position(int px, int py) : x(px), y(py) {}

	bool operator==(const Position& other) const {
		return x == other.x && y == other.y;
	}
};

struct PosHash {
	std::size_t operator()(const Position& p) const {
		std::size_t h1 = std::hash<int>{}(p.x);
		std::size_t h2 = std::hash<int>{}(p.y);
		return h1 ^ (h2 << 1); // muss up the Y bits 
	}			       // and then XOR w/ X to create unique hash
};

struct PosData {
	Combatant*         occupant = nullptr;
	vector<Combatant*> incoming;
};

using Battlefield = unordered_map<Position, PosData, PosHash>;




#endif
