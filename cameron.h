#pragma once
#include <iostream>
#include "group.h"
#include "coset.h"
#include "permutation.h"
#include "multi.h"
#include "everything.h"

class CameronReduction {
	RestrictedNaturalSetAction phi;
	std::deque<std::vector<int>> B;
public:
	CameronReduction( RestrictedNaturalSetAction phi_, std::deque<std::vector<int>>&& B_ ) : phi(phi_), B(B_) {}
	Iso operator() ( Group H, string x, string y ) {
		std::cout << H->generators() << std::endl;
		std::cout << x << std::endl;
		std::cout << y << std::endl;
		// std::cin.get();
		return Empty();
	}
};

// change delta to vector?
template<typename T>
std::deque<std::vector<int>> CameronIdentificationPart( const std::vector<std::array<int,2>>& Gamma_prime, const std::unordered_map<int,std::deque<int>>& Delta, size_t n ) {
	#ifdef DEBUG
	int counter = 0;
	#endif
	std::vector<bool> B(n);
	std::vector<bool> C_prime(n);
	size_t setsize = 0;
	std::vector<int> C;
	std::deque<std::vector<int>> D_prime;
	for( const auto& gamma : Gamma_prime ) {
		#ifdef DEBUG
		if( (++counter) % 100 == 0 )
			std::cerr << counter << std::endl;
		#endif
		int x = gamma[0];
		int y = gamma[1];
		B.assign( n, false );
		C_prime.assign( n, true );
		for( int q : Delta.at(y) )
			B[q] = true;
		for( int q : Delta.at(x) )
			B[q] = false;
		for( int z = 0; z < n; z++ )
			if( B[z] )
				for( int q : Delta.at(z) )
					C_prime[q] = false;
		C.reserve( setsize );
		for( int i = 0; i < n; i++ )
			if( C_prime[i] )
				C.push_back( i );
		// C is sorted
		setsize = C.size();
		if( std::find( D_prime.begin(), D_prime.end(), C ) == D_prime.end() )
			D_prime.emplace_back( std::move( C ) );
	}
	return D_prime;
}

template<typename T>
Iso CameronIdentification( RestrictedNaturalSetAction phi, string x, string y, T f ) {
	// step 0: consider action on blocks as points
	Group G = phi.anonymize();
	size_t n = G->degree();

	// step 1: check if G is a giant
	if( G->isGiant() ) {
		std::deque<std::vector<int>> B;
		for( size_t i : range(0,n) )
			B.emplace_back( 1, i );
		return CameronReduction( phi, std::move( B ) )( G, x, y );
	}

	// step 2: calculate the orbitals
	NaturalArrayAction<2> A( G );
	auto orbitals = A.orbits();
	std::sort( orbitals.begin(), orbitals.end(), size_compare<std::vector<NaturalArrayAction<2>::value_type>> );

	#ifdef DEBUG
	std::cerr << "orbitals done" << std::endl;
	#endif

	// identify the relations
	const auto& Sigma_1 = orbitals[1];
	const auto& Phi = orbitals.back();

	#ifdef DEBUG
	std::cerr << Sigma_1 << std::endl << Phi << std::endl;
	#endif

	// turn Phi into a map
	std::unordered_map<int,std::deque<int>> Delta;
	for( std::array<int,2> phi : Phi )
		Delta[phi[0]].push_back( phi[1] );

	// split Sigma_1
	std::deque<std::vector<std::array<int,2>>> Gamma = split( Sigma_1, THREADS );

	// step 3: delegate

	#ifdef THREADED
	std::future<std::deque<std::vector<int>>> results[THREADS-1];
	{
		size_t i = 0;
		for( auto itr = Gamma.begin()+1; itr != Gamma.end(); ++itr, ++i )
			results[i] = std::async( CameronIdentificationPart<T>, std::cref( Gamma[i+1] ), std::cref( Delta ), n );
	}
	#endif

	auto primary_result = CameronIdentificationPart<T>( Gamma[0], Delta, n );

	#ifdef THREADED
	for( size_t i = 0; i < THREADS-1; ++i ) {
		auto secondary_result = results[i].get();
		std::deque<std::vector<int>> intermediate;
		std::set_union( primary_result.begin(), primary_result.end(), secondary_result.begin(), secondary_result.end(), intermediate.begin() );
		primary_result = std::move( intermediate );
	}
	#endif

	// step 4: identify block
	std::map<int,std::vector<int>> E;
	const auto d = primary_result[0];
	size_t i = 1;
	for( const auto& e : primary_result )
		E[ intersection_size( d, e ) ].push_back( i++ );

	std::deque<std::vector<int>> B;
	size_t minimal_intersection_size = E.begin()->first;
	for( int p : E.begin()->second ) {
		std::vector<int> v( primary_result[p].begin(), primary_result[p].end() );
		B.push_back( std::move( v ) );
	}

	// step 5: stabilise block
	NaturalSetAction C( G, G->degree(), primary_result[0].size() );
	Group H( new Subgroup( G, 
		[&]( const Permutation& sigma ) -> bool { 
			return intersection_size( d, C( sigma, d ) ) == minimal_intersection_size; 
		} ) );
	   // sigma stabilises B iff it maps a point from B, here d, to somewhere in B iff d intersects sigma d minimally

	// step 6:
	return WeakReduction( G, H, x, y, CameronReduction( phi, std::move( B ) ) );
}