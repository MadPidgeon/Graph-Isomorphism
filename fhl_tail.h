#pragma once
#include <set>
#include <vector>
#include <utility>
#include <deque>
#include "group.h"
#include "permutation.h"

class FurstHopcroftLuksTail {
	typedef std::pair<Permutation,Permutation> team;
	Group G;
	Group H;
	std::vector<std::set<team>> reps;
	int n;
	bool filter( Permutation, bool add = true );
public:
	FurstHopcroftLuks() : G( nullptr ), H( nullptr ) {}
	void create( Group groupG, Group groupH, std::deque<team> seed );
};

