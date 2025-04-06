//
// Created by J. Blackburn - Apr 6 2025
//

#ifndef RANGED_H
#define RANGED_H

#include "Combatant.h"

class Ranged : public Combatant {
public:
	Ranged(Position p, Color c, int o) : Combatant(p, c, o) {}

protected:
	const Traits& getTraits() const override { return traits; }

private:
	static const Traits traits;
};

#endif
