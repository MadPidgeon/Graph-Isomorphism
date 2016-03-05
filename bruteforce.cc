#include <string>
#include <queue>
#include <set>
#include "group.h"
#include "coset.h"

Iso BruteforceIsomorphism( Group G, std::string x, std::string y ) {
	// find isomorphism
	int n = G.order();
	const auto& gens = G.generators();
	Permutation tau;
	std::queue<Permutation> next;
	std::set<Permutation> tried;
	next.push( G->one() )
	while( next.size() ) {
		for( const Permutation& sigma : gens ) {
			tau = gens.front() * sigma;
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
	return Coset( G, new Subgroup( G, {G->one()} ), tau );
}
