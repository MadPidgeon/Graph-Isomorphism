#pragma once
#include <iostream>
#include <cassert>

class Coset;

#include "group.h"
#include "permutation.h"
#include "ext.h"

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
Coset operator*( const Permutation& sigma, const Coset& tauH );

struct Iso : public Either<Coset,Empty> {
	bool isEmpty() const;
	const Coset& coset() const;
	using Either::Either;
};

Iso operator*( const Permutation& sigma, const Iso& tauH );
