#include <deque>
#include <vector>
#include <string>

#include "group.h"
#include "action.h"
#include "coset.h"
#include "fhl.h"
#include "cameron.h"

Iso StringIsomorphism( Group G, string x, string y ) {
	#ifdef DEBUG
	std::cerr << "StringIsomorphism(" << G->generators() << "," << x << "," << y << "):" << std::endl;
	#endif
	bool isAut = true;
	for( auto sigma : G->generators() ) {
		if( stringAction( sigma, x ) != x ) {
			isAut = false;
			break;
		}
	}
	if( isAut ) {
		std::cerr << "isAut" << std::endl;
		if( x == y )
			return Coset( G, G, G->one(), false );
		else
			return Empty();
	} else 
		return StringIsomorphismNonautomorphism( G, x, y );
}

Iso StringIsomorphismNonautomorphism( Group G, string x, string y ) {
	#ifdef DEBUG
	std::cerr << "StringIsomorphismNonautomorphism:" << std::endl;
	#endif
	NaturalAction A( G );
	auto Delta = A.orbits();
	if( Delta.size() == 1 )
		return StringIsomorphismTransitive( G, x, y );
	else
		return ChainRule( G, x, y, Delta, StringIsomorphismNonTransitive( /*std::move(Delta)*/ ) );
}

Iso StringIsomorphismTransitive( Group G, string x, string y ) {
	#ifdef DEBUG
	std::cerr << "StringIsomorphismTransitive( " << G->generators() << "," << x << "," << y << "):" << std::endl;
	#endif
	NaturalAction C( G );
	auto A = C.systemOfImprimitivity();
	auto B = A.domain();
	int m = B.size();
	auto H = A.anonymize();
	std::cout << B << std::endl;
	// checkkkkkk (2(log(n))^2)^(2*(log(n))^2)*(log(n)!) <= exp(4*log(n)*log(n)*log(log(n)))
	std::cout << double( H->order() ) << "  " << std::pow( m, 1 + log2( m ) ) << std::endl;
	if( H->order() < std::pow( m, 1 + log2( m ) ) ) { // or H->degree() <= 24
		auto N = A.kernel();
		return WeakReduction( G, N, x, y, StringIsomorphism );
	} else {
		return StringIsomorphismCameronGroup( A, H, x, y );
	}
}

Iso StringIsomorphismCameronGroup( RestrictedNaturalSetAction A, Group H, string x, string y ) {
	#ifdef DEBUG
	std::cerr << "StringIsomorphismCameronGroup( " << H->generators() << "," << x << "," << y << "):" << std::endl;
	#endif
	return CameronIdentification( A, x, y, Empty() );
	/*auto C = CameronStructure( H );
	auto J = H;
	return WeakReduction( C, J, string x, string y, StringIsomorphismJohnsonGroup );*/
	return Coset( H, H, H->one(), false );
}


int main() {
	std::string x = "aaab";
	std::string y = "baaa";
	Group S4( new SymmetricGroup( 4 ) );
	Group G( new Subgroup( S4, { {1,0,3,2}, {0,2,1,3} } ) );
	std::cout << StringIsomorphism( G, x, y ) << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	Group S24( new SymmetricGroup( 24 ) );
	NaturalSetAction A( S24, 24, 2 );
	auto S24_2 = A.anonymize();
	std::string u = "abababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababab";
	std::string v = "babababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababa";
	std::cout << StringIsomorphism( S24_2, u, v ) << std::endl;
	return 0;
}