#include "group.h"
#include "action.h"

std::set<int> JordanLiebeckSet( Action<std::set<int>> phi, int x ) {
	Group Gx = phi.group().stabilizer(x);
	Action<std::set<int>> phi_prime = phi.subgroupAction( Gx );
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
	std::set<int> Delta;
	Delta.reserve( n - largest_size );
	for( size_t i = 0; i < O.size(); i++ )
		if( i != largest )
			Delta.insert( Delta.back(), O[i].begin(), O[i].end() );
	std::sort( Delta.begin(), Delta.end() );
	return Delta;
}

std::vector<std::set<int>> JohnsonStandardBlocks( Action<std::set<int>> phi ) {
	Group G = phi.group();
	auto& Omega = G.domain();
	std::map<std::set<int>,std::set<int>> equivalence_map;
	for( int x : Omega ) {
		std::set<int> Tx = JordanLiebeckSet( x );
		if( equivalence_map.count( Tx ) == 0 )
			equivalence_map.insert( Tx, std::set<int>( x ) );
		else
			equivalence_map[Tx].insert( x );
	}
	std::vector<std::set<int>> blocks;
	blocks.reserve( equivalence_map.size() );
	for( auto& partition : equivalence_map )
		blocks.emplace_back( std::move( partition.second ) );
	return NaturalAction( G ).setwiseAction( blocks );
}