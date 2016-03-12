#pragma once
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <utility>
#include <future>
#include "group.h"
#include "action.h"
#include "ext.h"


/*std::set<std::set<int>> CameronThread( std::vector<std::array<int,2>>::const_iterator begin, std::vector<std::array<int,2>>::const_iterator end, const std::unordered_map<int,std::vector<int>>& Delta ) {
	int counter = 0;
	std::unordered_set<int> B;
	std::set<int> C;
	std::set<std::set<int>> D_prime;
	for(; begin != end; ++begin ) {
		if( (++counter) % 10 == 0 )
			std::cerr << counter << std::endl; 
		int x = (*begin)[0];
		int y = (*begin)[1];
		B = {};
		C = {};
		for( int q : Delta.at(y) )
			B.insert( q );
		for( int q : Delta.at(x) )
			B.erase( q );
		// C.insert( phi.domain().begin(), phi.domain().end() );
		for( int z : B )
			for( int q : Delta.at(z) )
				C.insert( q );
		D_prime.emplace( std::move( C ) );
	}
	return D_prime;
}*/

std::vector<int> JordanLiebeckSet( RestrictedNaturalSetAction, int x );
RestrictedNaturalSetAction JohnsonStandardBlocks( RestrictedNaturalSetAction phi );
RestrictedNaturalSetAction CameronReduction( NaturalAction phi );
