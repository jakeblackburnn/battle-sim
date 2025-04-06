//
// Created by J. Blackburn - Apr 6 2025
//

#ifndef INTEL_H
#define INTEL_H

#include "Combatant.h"

class Intel : public Combatant {
public:
	Intel(Position p, Color c, int o) : Combatant(p, c, o) {}

protected: 
	const Traits& getTraits() const override { return traits; }

private:
	static const Traits traits;
};

#endif
