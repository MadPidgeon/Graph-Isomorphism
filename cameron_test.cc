#include "action.h"
#include "johnson.h"
#include "group.h"

int main() {
	// construct elements: 2-transversals
	Group S18( new SymmetricGroup( 2*16 ) );
	RestrictedNaturalSetAction::domain_type D;
	for( auto& t1 : all_ordered_tuples( 16, 2 ) ) {
		for( auto& t2 : all_ordered_tuples( 16, 2) ) {
			RestrictedNaturalSetAction::value_type V;
			V.reserve( 4 );
			V.push_back( t1[0] );
			V.push_back( t1[1] );
			V.push_back( t2[0] + 16 );
			V.push_back( t2[1] + 16 );
			D.emplace_back( std::move( V ) );
		}
	}
	// construct group: S_16 Wr C_2
	Group G( new Subgroup( S18, {
		{ 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15, 0, 16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31},
		{ 1, 0, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15, 16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31},
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15, 17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,16},
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15, 17,16,18,19,20,21,22,23,24,25,26,27,28,29,30,31},
		{16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12,13,14,15}} ) );
	// anonimize the action of G on D
	RestrictedNaturalSetAction A( G, D );
	Group G_prime = A.anonymize();
	std::cout << G_prime->generators() << std::endl;
	// find structure
	auto C = CameronReduction( NaturalAction( G_prime ) );

	// creating a wreath product
	/*Group S18( new SymmetricGroup( 16+16 ) );
	std::vector<std::set<int>> subsets;
	std::vector<std::vector<std::set<int>>> transversals;
	subsets.reserve( 120 );
	for( const auto& t : all_ordered_tuples( 16, 2 ) )
		subsets.emplace_back( t.begin(), t.end() );
	for( const auto& t : all_tuples( 120, 2 ) ) {
		std::vector<std::set<int>> transversal;
		for( int i : t )
			transversal.push_back( subsets[i] );
		transversals.emplace_back( std::move( transversal ) );
	}
	std::cout << "----------------" << std::endl;
	// S_16 wr C_2
	Group G( new Subgroup( S18, {
		{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0, 16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 32,33},
		{1,0,2,3,4,5,6,7,8,9,10,11,12,13,14,15, 16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 32,33},
		{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15, 17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,16, 32,33},
		{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15, 17,16,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 32,33},
		{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15, 16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 33,32}} ) );
	Action<std::vector<std::set<int>>> A( G, transversals, [] ( const Permutation& sigma, const std::vector<std::set<int>>& S ) -> std::vector<std::set<int>> {
		std::vector<std::set<int>> T( S.size() );
		for( int i = 0; i < 2; i++ ) {
			int i2 = sigma(i+32)-32;
			for( int j : S[i] )
				T[i2].insert( sigma(j+i2*16) % 16 );
		}
		return T;
	} );
	Permutation sigma({1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0, 17,16,18,19,20,21,22,23,24,25,26,27,28,29,30,31, 33,32});
	std::vector<std::set<int>> t = transversals[0];
	for( int i = 0; i < 50; i++ ) {
		std::cout << t << std::endl;
		t = A( sigma, t );
	}*/

	/*Group S18( new SymmetricGroup( 5+5+5+3 ) );
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
	std::cout << "----------------" << std::endl;
	// S_5 wr C_3
	Group G( new Subgroup( S18, {
		{1,2,3,4,0,5,6,7,8,9,10,11,12,13,14,15,16,17},
		{1,0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17},
		{0,1,2,3,4,6,7,8,9,5,10,11,12,13,14,15,16,17},
		{0,1,2,3,4,6,5,7,8,9,10,11,12,13,14,15,16,17},
		{0,1,2,3,4,5,6,7,8,9,11,12,13,14,10,15,16,17},
		{0,1,2,3,4,5,6,7,8,9,11,10,12,13,14,15,16,17},
		{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,16,17,15},
		{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,16,15,17}} ) );
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
	}*/
	/*std::cout << "----------------" << std::endl;
	auto Q = A.systemOfImprimitivity();
	std::cout << Q.domain() << std::endl;
	std::cout << "----------------" << std::endl;
	Group H = A.anonymize();
	std::cout << H->domain().size() << std::endl;
	//std::cout << H->degree() << ", " << H->order() << std::endl;
	std::cout << "----------------" << std::endl;
	/*auto C = CameronReduction(A);
	std::cout << C.domain() << std::endl;*/
	/*Action<int> B = NaturalAction( H );
	auto C = CameronReduction( B );
	std::cout << C.domain() << std::endl;*/

	return 0;
}