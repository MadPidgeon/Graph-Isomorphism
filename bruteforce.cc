#include <string>
#include <queue>
#include <set>
#include "group.h"
#include "coset.h"

std::string StringAction( const Permutation& sigma, std::string x ) {
	std::string y = x;
	for( size_t i = 0; i < x.size(); i++ )
		y[i] = x[sigma(i)];
	return y;
}

Iso BruteforceIsomorphism( Group G, std::string x, std::string y ) {
	// find isomorphism
	auto gens = G->generators();
	Permutation tau = G->one();
	std::queue<Permutation> next;
	std::set<Permutation> tried;
	next.push( tau );
	while( next.size() ) {
		for( const Permutation& sigma : gens ) {
			tau = next.front() * sigma;
			if( !tried.count( tau ) ) {
				if( StringAction( tau, y ) == x )
					goto BruteforceIsomorphism_FoundIsomorphism;
				tried.insert( tau );
				next.push( tau );
			}
		}
		next.pop();
	}
	return Empty();
BruteforceIsomorphism_FoundIsomorphism:
	Permutation mu = G->one();
	std::deque<Permutation> elements;
	tried.clear();
	next = {};
	next.push( mu );
	while( next.size() ) {
		for( const Permutation& sigma : gens ) {
			mu = next.front() * sigma;
			if( !tried.count( mu ) ) {
				if( StringAction( mu, x ) == x )
					elements.push_back( mu );
				tried.insert( mu );
				next.push( mu );
			}
		}
		next.pop();
	}
	FurstHopcroftLuks compressor;
	compressor.create( elements );
	return Coset( G, Group( new Subgroup( G, compressor.generators() ) ), tau );
}
