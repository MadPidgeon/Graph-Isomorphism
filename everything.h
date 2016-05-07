#pragma once
#include <deque>
#include <vector>
#include <string>

#include "fhl.h"
#include "group.h"
#include "action.h"
#include "coset.h"


using std::string;

template<typename T>
string stringRestrict( const string& x, const T& Delta ) {
	string y;
	y.reserve( Delta.size() );
	for( int i : Delta )
		y.push_back( x[i] );
	return y;
}

string stringAction( const Permutation& sigma, const string& x ) {
	string y( x.size(), '*' );
	for( size_t i = 0; i < x.size(); ++i )
		y[sigma(i)] = x[i];
	return y;
}

template<typename T>
Iso ShiftIdentity( Coset C, string x, string y, T f );
template<typename T>
Iso WeakReduction( Group G, Group H, string x, string y, T f );
template<typename T>
Iso ChainRule( Group G, string x, string y, std::vector<std::vector<int>> orbits, T f );
Iso StringIsomorphism( Group G, string x, string y );
Iso StringIsomorphismNonautomorphism( Group G, string x, string y );
Iso StringIsomorphismTransitive( Group G, string x, string y );
Iso StringIsomorphismCameronGroup( RestrictedNaturalSetAction A, Group H, string x, string y );

class StringIsomorphismNonTransitive {
	//std::deque<std::deque<int>> orbits;
public:
	StringIsomorphismNonTransitive( /*std::deque<std::deque<int>>&& o*/ ) /*: orbits( std::move( o ) )*/ {}
	Iso operator()( Group G, string x, string y ) const {
		/*auto mu = G.one();
		for( const auto& Delta : orbits ) {
			auto perm = G.generators();
			for( auto& sigma : perm )
				sigma = sigma.project( Delta );
			Group H( perm );
			Iso I = StringIsomorphismTransitive( H, stringRestrict( x, Delta ), stringRestrict( y, Delta ) );
			if( I.isEmpty() )
				return Empty();
			PullbackStructure P( G.generators(), perm );
			auto tau = P.pullback( I.representative() );
			auto perm2 = I.group().generators();
			for( auto& sigma : perm2 )
				sigma = P.pullback( sigma );
			auto J = G.pointwiseStabiliser( Delta );
			Group K( new Subgroup( G.supergroup(), perm2 ) );
			G = J.join( K );
			y = tau.inverse() * y;
			mu = mu * tau;
		}*/
		return StringIsomorphismTransitive( G, x, y );
	}
};

template<typename T>
Iso ShiftIdentity( Coset C, string x, string y, T f ) {
	return C.representative() * f( C.subgroup(), x, stringAction( C.representative().inverse(), y ) );
}

template<typename T>
Iso WeakReduction( Group G, Group H, string x, string y, T f ) {
	#ifdef DEBUG
	std::cerr << "WeakReduction( " << G->generators() << "," << H->generators() << "," << x << "," << y << "):" << std::endl;
	#endif
	Group I;
	std::deque<Permutation> generators;
	for( Coset C : G->allCosets( H, false ) ) {
		//std::cout << "coset: " << C << std::endl;
		Iso s = ShiftIdentity( C, x, y, f );
		if( not s.isEmpty() ) {
			I = s.coset().subgroup();
			generators.push_back( s.coset().representative() );
		}
	}
	if( generators.size() == 0 )
		return Empty();
	Permutation sigma = generators.back();
	generators.pop_back();
	for( Permutation& tau : generators )
		tau = sigma.inverse() * tau;
	I = I->join( std::move( generators ) );
	return Coset( G, I, sigma, false );
}

template<typename T>
Iso ChainRule( Group G, string x, string y, std::vector<std::vector<int>> orbits, T f ) {
	#ifdef DEBUG
	std::cerr << "StringIsomorphismChainRule( " << G->generators() << "," << x << "," << y << "," << orbits << "):" << std::endl;
	#endif
	auto mu = G->one();
	Group F = G;
	for( const auto& Delta : orbits ) {
		auto perm = F->generators();
		std::deque<int> almostDelta( Delta.begin(), Delta.end() );
		for( auto& sigma : perm )
			sigma = sigma.project( Delta );
		Group H( new Subgroup( Group( new SymmetricGroup( Delta.size() ) ), perm ) );
		Iso I = f( H, stringRestrict( x, Delta ), stringRestrict( y, Delta ) );
		if( I.isEmpty() )
			return Empty();
		//std::cout << "F=" << F->generators() << std::endl;
		PullbackStructure P( F, perm, F->generators() );
		auto tau = P( I.coset().representative() );
		auto perm2 = I.coset().subgroup()->generators();
		std::deque<Permutation> perm3;
		for( auto& sigma : perm2 )
			perm3.push_back( P( sigma ) );
		//std::cout << perm3 << std::endl;
		//std::cout << tau << std::endl;


		//auto J = F->pointwiseStabiliser( Delta );
		RestrictedNaturalAction A( F, almostDelta );
		Group J = A.kernel();
		//std::cout << J->generators() << std::endl;
		// Group K( new Subgroup( F->supergroup(), perm2 ) );
		F = J->join( std::move( perm3 ) );
		//std::cout << F->generators() << std::endl;
		y = stringAction( tau.inverse(), y );
		mu = mu * tau;
		//std::cout << "lolwat" << std::endl;
	}
	Coset R( G, F, mu, false );
	#ifdef DEBUG
	std::cerr << "StringIsomorphismChainRule -> " << R << std::endl;
	#endif
	return R; 
}