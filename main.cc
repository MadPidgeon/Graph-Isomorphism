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
	Action<int> G_action = NaturalAction( G );
	std::cout << G_action( sigma, 5 ) << std::endl;
	std::cout << G_action( tau, 1 ) << std::endl;
	Action<int> H_action = NaturalAction( H );
	std::cout << H_action.isTransitive() << std::endl;
	for( auto& orbit : H_action.orbits() ) {
		for( int x : orbit )
			std::cout << x << " ";
		std::cout << std::endl;
	}
	std::cout << "------------------------------" << std::endl;
	Action<int> I_action = NaturalAction( I );
	std::cout << I_action.isTransitive() << std::endl;
	for( auto& orbit : I_action.orbits() ) {
		for( int x : orbit )
			std::cout << x << " ";
		std::cout << std::endl;
	}
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
	std::vector<int> domain1 = {0,1,2,3};
	Action<int> action1 = SubsetAction( H, domain1 );
	Group K = action1.kernel();
	std::cout << K->generators() << std::endl;
	std::cout << "------------------------------" << std::endl;
	for( auto tup : all_tuples(6,3) )
		std::cout << tup << std::endl;
	std::cout << "---" << std::endl;
	for( auto tup : all_ordered_tuples(6,3) )
		std::cout << tup << std::endl;
}
