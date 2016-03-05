#include <iostream>
#include "group.h"
#include "coset.h"

#define C0	9

Iso AggregateGenerators( Group U, Group G, std::vector<Permutation> S ) {
	if( S.empty() )
		return Empty();
	Permutation rho = S.back();
	Permutation rho_inv = rho.inverse();
	S.pop_back();
	for( auto& sigma : S )
		sigma *= rho_inv;
	for( auto& tau : G.generators() )
		S.push_back( tau );
	Group H( new Subgroup( U, S ) );
	return Coset( U, H, rho );
}

Iso WeakReduction( Action<std::set<int>> B ) {
	Group G = B.group();
	Action<std::set<int>> C = MagicalJohnson( B );
	Group N = C.group();
	std::vector<Permutation> S;
	for( auto H : G.allCosets( N ) ) {
		auto result = JohnsonCase( C, x, stringAction( H.representative(), y ) );
		if( result.isFirst() )
			S.push_back( result.getFirst().representative() * H.representative() );
	}
	return AggregateGenerators( G, N, S );
}

Iso StrongReduction( Action<std::set<int>> A, const std::string& x, const std::string& y ) {
	Group G = A.group();
	Group N = A.kernel();
	Group T;
	Group R;
	Permutation tau;
	std::string y2;
	std::vector<Permutation> S;
	std::vector<std::vector<int>> blocks;
	bool succes;
	for( const auto& block : A.domain() )
		blocks.emplace_back( block.begin(), block.end() );
	for( auto& H : G.allCosets( N ) ) {
		T = G;
		tau = T->one();
		y2 = stringAction( y, H.representative() );
		succes = true;
		for( auto& block : blocks ) {
			Iso result = ReducedDomainIsomorphism( G, x, y2, block );
			if( result.isSecond() ) {
				succes = false;
				break;
			} else {
				T = result.getFirst().group()
				tau = result.getFirst().representative() * tau;
				y2 = stringAction( result.getFirst().representative(), y2 );
			}
		}
		if( succes ) {
			S.push_back( tau );
			R = T;
		}
	}
	return AggregateGenerators( G, R, S );
}

Iso ReducedDomainIsomorphism( Group G, const std::string& x, const std::string& y, std::vector<int> domain ) {
	// cheat
	std::string x2, y2;
	for( int i : domain ) {
		x2.push_back( x[i] );
		y2.push_back( y[i] );
	}
	return StringIsomorphism( G, x2, y2 ); // does not work
}

Iso StringIsomorphism( Group G, const std::string& x, const std::string& y ) {
	// line 1.
	Group Aut = AutomorphismGroup( x );
	if( Aut.hasSubgroup( G ) ) {
		if( x == y )
			return Iso( Coset( G, G, G->one() ) );
		else
			return Iso( Empty() );
	}
	// line 2.
	if( G.order() < C0 )
		return BruteforceIsomorphism( G, x, y );
	// line 3.
	auto A = NaturalAction( G );
	if( !A.isTransitive() )
		return ChainRule( G, x, y );
	// line 4.
	auto B = A.systemOfImprimitivity();
	const auto& Gamma = B.domain();
	int m = Gamma.size();
	// line 5.
	if( B.order() < pow(m,1+log2(m)) )
		return StrongReduction( B, x, y );
	// line 6.
	return WeakReduction( B, x, y );
}


Iso JohnsonCase( Action<std::set<int>> B, const std::string& x, const std::string& y ) {
	Group G = B.group();
	const auto& Omega = G.domain();
	const auto& Gamma = B.domain(); 
	int n = x.size();
	int m = Gamma.size();
	int l = log( n );
	l = l*l*l;

	// line 2.
	if( m <= l )
		return StrongReduction( B, x, y );
	// line 3.
	Action<std::set<int>> A = JohnsonStandardBlocks( B );
	auto& Phi = A.domain();
	int t = Phi.begin()->size();
	// line 4.
	if( Phi.size() == Omega.size() ) {
		if( t == 1 ) {
			return TrivialIsomorphism( G, x, y );
		} else {

		}
	} else {

	}
}