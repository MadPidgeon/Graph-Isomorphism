#include <vector>
#include <set>
#include <map>

#include "group.h"
#include "action.h"
#include "ext.h"
#include "multi.h"
/*
std::deque<std::vector<int>> CameronReductionPart( std::vector<std::array<int,2>>::const_iterator beg, std::vector<std::array<int,2>>::const_iterator end, const std::unordered_map<int,std::deque<int>>& Delta, int n ) {
	int counter = 0;
	std::vector<bool> B(n);
	std::vector<bool> C_prime(n);
	size_t setsize = 0;
	std::vector<int> C;
	std::deque<std::vector<int>> D_prime;
	for( ; beg != end; beg++ ) {
		if( (++counter) % 100 == 0 )
			std::cerr << counter << std::endl; 
		int x = beg->at(0);
		int y = beg->at(1);
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
		setsize = C.size();
		if( std::find( D_prime.begin(), D_prime.end(), C ) == D_prime.end() )
			D_prime.emplace_back( std::move( C ) );
	}
	return D_prime;
}

RestrictedNaturalSetAction CameronReduction( NaturalAction phi ) {
	int n = phi.domain().size();
	NaturalArrayAction<2> psi( phi.group() );
	auto orbitals = psi.orbits();
	std::cerr << "orbitals done" << std::endl;
	std::sort( orbitals.begin(), orbitals.end(), size_compare<std::vector<NaturalArrayAction<2>::value_type>> );
	const auto& Gamma = orbitals[1];
	const auto& Delta_prime = orbitals.back();
	std::unordered_map<int,std::deque<int>> Delta;
	for( const auto& delta : Delta_prime )
		Delta[delta[0]].push_back( delta[1] );
	std::deque<std::vector<int>> D_prime;
	std::cerr << "|Gamma|=" << Gamma.size() << std::endl; 
	std::cerr << "|Delta|=" << Delta_prime.size() << std::endl; 
	#ifdef THREADED
	std::cerr << THREADS << std::endl;
	std::future<std::deque<std::vector<int>>> fut[THREADS];
	auto beg = Gamma.cbegin();
	auto end = Gamma.cbegin();
	for( size_t i = 0; i < THREADS-1; i++ ) {
		end += Gamma.size()/THREADS;
		fut[i] = std::async( CameronReductionPart, beg, end, Delta, n );
		beg = end;
	}
	std::deque<std::vector<int>> res = CameronReductionPart( end, Gamma.cend(), std::cref( Delta ), n );
	for( size_t i = 0; i < THREADS-1; i++ ) {
		std::deque<std::vector<int>> res2 = fut[i].get();
		for( auto& r : res2 )
			if( std::find( res.begin(), res.end(), r ) == res.end() )
				res.emplace_back( std::move( r ) );
	}
	#else
	std::deque<std::vector<int>> res = CameronReductionPart( Gamma.cbegin(), Gamma.cend(), std::cref( Delta ), n );
	#endif
	std::cout << res << std::endl;
	RestrictedNaturalSetAction chi( std::move( res ) );
	RestrictedNaturalSetAction psi = chi.reverseSystemOfImprimitivity();


	std::cout << "Done" << std::endl;
	return RestrictedNaturalSetAction( phi.group(), D_prime );


	/*std::vector<std::set<T>> D( D_prime.begin(), D_prime.end() );
	Action<std::set<T>> chi_prime = phi.setwiseAction( D );
	std::cerr << chi_prime.domain() << std::endl; 
	Action<std::set<std::set<T>>> chi = chi_prime.reverseSystemOfImprimitivity();
	std::cerr << chi.domain() << std::endl; 
	D.clear();
	for( const auto& ss : chi.domain() )
		D.emplace_back( std::move( flatten( ss ) ) );
	return Action<std::set<T>>( chi_prime.group(), D, chi_prime.function() );*/

	//return RestrictedNaturalSetAction( phi.group(), std::deque<std::vector<int>>( 1, std::vector<int>( 1, 0 ) ) );
//}

/*std::vector<int> JordanLiebeckSet( RestrictedNaturalSetAction phi, int x ) {
	Group Gx = phi.group().stabilizer(x);
	RestrictedNaturalSetAction phi_prime( Gx, phi.domain() );
	auto O = phi_prime.orbits();
	int largest_size = 0;
	int largest = -1;
	int n = 0;
	for( size_t i = 0; i < O.size(); i++ ) {
		n += O[i].size();
		if( O[i].size() > largest_size ) {
			largest_size = O[i].size();
			largest = i;
		}
	}
	std::vector<int> Delta;
	Delta.reserve( n - largest_size );
	for( size_t i = 0; i < O.size(); i++ )
		if( i != largest )
			Delta.insert( Delta.back(), O[i].begin(), O[i].end() );
	std::sort( Delta.begin(), Delta.end() );
	return Delta;
}

RestrictedNaturalSetAction JohnsonStandardBlocks( RestrictedNaturalSetAction phi ) {
	Group G = phi.group();
	const auto& Omega = G.domain();
	std::map<std::vector<int>,std::vector<int>> equivalence_map;
	for( int x : Omega ) {
		std::set<int> Tx = JordanLiebeckSet( phi, x );
		if( equivalence_map.count( Tx ) == 0 )
			equivalence_map.insert( Tx, std::vector<int>({ x }) );
		else
			equivalence_map[Tx].push_back( x );
	}
	std::deque<std::vector<int>> blocks;
	for( auto& partition : equivalence_map )
		blocks.emplace_back( std::move( partition.second ) );
	return RestrictedNaturalSetAction( G, blocks );
}*/

std::vector<std::vector<int>> JordanLiebeckSet( RestrictedNaturalSetAction phi, int x ) {
	Group Gx = phi.group()->stabilizer(x);
	// std::cout << Gx->generators() << std::endl;
	RestrictedNaturalSetAction phi_prime( Gx, phi.domain() );
	std::vector<std::vector<std::vector<int>>> O = phi_prime.orbits();
	// std::cout << O << std::endl;
	size_t largest_size = 0;
	size_t largest = 0;
	int n = 0;
	for( size_t i = 0; i < O.size(); i++ ) {
		n += O[i].size();
		if( O[i].size() > largest_size ) {
			largest_size = O[i].size();
			largest = i;
		}
	}
	std::vector<std::vector<int>> Delta;
	// Delta.reserve( n - largest_size );
	for( size_t i = 0; i < O.size(); i++ )
		if( i != largest )
			Delta.insert( Delta.end(), O[i].begin(), O[i].end() );
	std::sort( Delta.begin(), Delta.end() );
	return Delta;
}


RestrictedNaturalSetAction JohnsonStandardBlocks( RestrictedNaturalSetAction phi ) {
	Group G = phi.group();
	const auto& Omega = G->domain();
	std::map<std::vector<std::vector<int>>,std::vector<int>> equivalence_map;
	for( int x : Omega ) {
		std::cout << x << std::endl;
		std::vector<std::vector<int>> Tx = JordanLiebeckSet( phi, x );
		std::cout << Tx << std::endl;
		equivalence_map[Tx].push_back( x );
		/*if( equivalence_map.count( Tx ) == 0 )
			equivalence_map.insert( Tx, std::vector<int>({ x }) );
		else
			equivalence_map[Tx].push_back( x );*/
	}
	std::deque<std::vector<int>> blocks;
	for( auto& partition : equivalence_map )
		blocks.emplace_back( std::move( partition.second ) );
	return RestrictedNaturalSetAction( G, blocks );
}