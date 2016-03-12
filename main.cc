#include <iostream>
#include "group.h"
#include "action.h"

int main() {
	Group S8( new SymmetricGroup(8) );
	Permutation sigma( {1,2,3,4,5,6,7,0} );
	Permutation sigma2( {4,2,3,0,5,6,7,1} );
	Permutation tau( {2,3,4,5,6,7,0,1} );
	Permutation mu( {1,0,2,3,4,5,6,7} );
	Group G( new Subgroup( S8, {sigma} ) );
	Group G2( new Subgroup( S8, {sigma2}) );
	Group H( new Subgroup( S8, {{1,2,3,0,4,5,6,7},{0,1,2,3,5,6,7,4}} ) );
	Group I( new Subgroup( S8, {{1,2,3,0,4,5,6,7},{0,1,2,3,5,6,7,4},{4,5,6,7,0,1,2,3}} ) );

	std::cout << sigma(5) << std::endl;
	std::cout << mu(1) << std::endl;
	std::cout << sigma << std::endl;
	std::cout << tau << std::endl;
	std::cout << G->order() << std::endl;
	std::cout << H->order() << std::endl;

	std::cout << "------------------------------" << std::endl;
	std::cout << G->contains( tau ) << std::endl; 
	std::cout << G->contains( mu ) << std::endl; 
	std::cout << G->contains( G->one() ) << std::endl;

	std::cout << "------------------------------" << std::endl;
	NaturalAction G_action( G );
	std::cout << G_action( sigma, 5 ) << std::endl;
	std::cout << G_action( tau, 1 ) << std::endl;
	NaturalAction H_action( H );
	std::cout << H_action.isTransitive() << std::endl;
	std::cout << H_action.orbits() << std::endl;
	std::cout << "------------------------------" << std::endl;
	NaturalAction I_action( I );
	std::cout << I_action.isTransitive() << std::endl;
	std::cout << I_action.orbits() << std::endl;
	std::cout << "---" << std::endl;
	auto induced_action = I_action.systemOfImprimitivity();
	std::cout << induced_action.domain() << std::endl;
	std::cout << "---" << std::endl;
	std::cout << induced_action( {1,2,3,0,4,5,6,7}, {0,1,2,3} ) << std::endl;
	std::cout << induced_action( {4,5,6,7,0,1,2,3}, {0,1,2,3} ) << std::endl;
	std::cout << std::set<int>() << std::endl;
	std::cout << "---" << std::endl;
	induced_action = NaturalAction( G2 ).systemOfImprimitivity();
	std::cout << induced_action.domain() << std::endl;
	std::cout << "------------------------------" << std::endl;
	Group J( new Subgroup( H, []( const Permutation& sigma ){ return (sigma(1) == 3 or sigma(1) == 1) and (sigma(3) == 1 or sigma(3) == 3 ); } ) );
	std::cout << J->generators() << std::endl;
	std::cout << "---" << std::endl;
	RestrictedNaturalAction::domain_type domain1 = {0,1,2,3};
	RestrictedNaturalAction action1( I, domain1 );
	Group K = action1.kernel();
	std::cout << K->generators() << std::endl;
	std::cout << "------------------------------" << std::endl;
	for( auto tup : all_tuples(6,3) )
		std::cout << tup << std::endl;
	std::cout << "---" << std::endl;
	for( auto tup : all_ordered_tuples(6,3) )
		std::cout << tup << std::endl;
}
