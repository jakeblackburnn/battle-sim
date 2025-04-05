//
// Created by J. Blackburn - Mar 27 2025
//

#ifndef ATTACK_H
#define ATTACK_H

#include "Combatant.h"

class Attack : public Combatant {
public:
	Attack(Position p, Color c, int o) : Combatant(p, c, o) {}

protected: 
	const Traits& getTraits() const override { return traits; }

private:
	static const Traits traits;
};

#endif
