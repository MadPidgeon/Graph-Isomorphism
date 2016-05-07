#include "group.h"
#include "permutation.h"
#include "fhl.h"
#include "action.h"

int main() {
	Group X( new SymmetricGroup( 10 ) );
	Group Y( new Subgroup( X, {{1,2,3,4,5,6,7,8,9,0},{5,4,3,2,1,0,9,8,7,6}} ) );
	Group Z( new Subgroup( Y, {{2,3,4,5,6,7,8,9,0,1}} ) );
	Group W( new Subgroup( Y, [&]( const Permutation& sigma ) { return Z->contains( sigma ); } ) );
	std::cout << Y->generators() << std::endl;
	std::cout << Z->generators() << std::endl;
	std::cout << W->generators() << std::endl;
	std::cout << Y->allCosets( Z, false ) << std::endl;
	std::cout << "----------------------------------" << std::endl;
	Group C( new SymmetricGroup( 10 ) );
	SubgroupGenerator sg( C, []( const Permutation& sigma ) -> bool { return sigma(0) < 2 and sigma(1) < 2; } );
	Group D = Group( new Subgroup( sg.subgroup() ) );
	// std::cout << sg << std::endl;
	std::cout << D->generators() << std::endl;
	std::cout << "----------------------------------" << std::endl;
	int n = 24;
	Group E( new SymmetricGroup( n ) );
	NaturalSetAction A( E, n, 2 );
	Group F = A.anonymize();
	std::cout << F->generators() << std::endl;
	FHL<> fhl1( F->generators() );
	// std::cout << fhl1 << std::endl;
	std::cout << "----------------------------------" << std::endl;
	Group G( new SymmetricGroup( 8 ) );
	Group H( new Subgroup( G, {{1,2,3,4,5,6,7,0}} ) );
	FHL<> fhl2( H->generators() );
	std::cout << "----------------------------------" << std::endl;
	Group I( new SymmetricGroup( 4 ) );
	std::vector<PermutationPullback> pullbacks;
	pullbacks.emplace_back( Permutation({1,2,3,0}), Permutation({1,2,3,4,5,6,7,0}) );
	PullbackStructure pullback_structure( H, pullbacks );
	Permutation sigma = pullback_structure({2,3,0,1});
	std::cout << sigma << std::endl;

	return 0;
}