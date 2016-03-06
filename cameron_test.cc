#include "action.h"
#include "johnson.h"
#include "group.h"

int main() {
	// creating a wreath product
	Group S18( new SymmetricGroup( 5+5+5+3 ) );
	std::vector<std::set<int>> subsets;
	std::vector<std::vector<std::set<int>>> transversals;
	subsets.reserve( 10 );
	for( const auto& t : all_ordered_tuples( 5, 2 ) )
		subsets.emplace_back( t.begin(), t.end() );
	for( const auto& t : all_tuples( 10, 3 ) ) {
		std::vector<std::set<int>> transversal;
		for( int i : t )
			transversal.push_back( subsets[i] );
		transversals.emplace_back( std::move( transversal ) );
	}
	std::cout << transversals << std::endl;
	std::cout << "----------------" << std::endl;
	// S_5 wr C_3
	Group G( new Subgroup( S18, {
		{1,2,3,4,0,5,6,7,8,9,10,11,12,13,14,15,16,17},
		{1,0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17},
		{0,1,2,3,4,6,7,8,9,5,10,11,12,13,14,15,16,17},
		{0,1,2,3,4,6,5,7,8,9,10,11,12,13,14,15,16,17},
		{0,1,2,3,4,5,6,7,8,9,11,12,13,14,10,15,16,17},
		{0,1,2,3,4,5,6,7,8,9,11,10,12,13,14,15,16,17},
		{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,16,17,15}} ) );
	Action<std::vector<std::set<int>>> A( G, transversals, [] ( const Permutation& sigma, const std::vector<std::set<int>>& S ) -> std::vector<std::set<int>> {
		std::vector<std::set<int>> T( S.size() );
		for( int i = 0; i < 3; i++ ) {
			int i2 = sigma(i+15)-15;
			for( int j : S[i] )
				T[i2].insert( sigma(j+i2*5) % 5 );
		}
		return T;
	} );
	Permutation sigma({1,2,3,4,0,6,5,7,8,9,10,11,12,13,14,16,17,15});
	std::vector<std::set<int>> t = transversals[0];
	for( int i = 0; i < 20; i++ ) {
		std::cout << t << std::endl;
		t = A( sigma, t );
	}
	std::cout << "----------------" << std::endl;
	Group H = A.anonymize();
	std::cout << H->generators() << std::endl;
	//std::cout << H->degree() << ", " << H->order() << std::endl;
	std::cout << "----------------" << std::endl;
	Action<int> B = NaturalAction( H );
	auto C = CameronReduction( B );
	std::cout << C.domain() << std::endl;

	return 0;
}