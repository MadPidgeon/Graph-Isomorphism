#pragma once
#include <iostream>

class Coset;

#include "group.h"
#include "permutation.h"

class Coset {
	Group _G;
	Group _H;
	bool _right;
	Permutation _sigma;
public:
	Group supergroup() const;
	Group subgroup() const;
	bool isRightCoset() const;
	const Permutation& representative() const;
	bool operator==( const Coset& ) const;
	Coset( Group G, Group H, Permutation sigma, bool right = true );
};

std::ostream& operator<<( std::ostream& os, const Coset& c );

typedef Either<Coset,Empty> Iso;
