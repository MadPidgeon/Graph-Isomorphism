#include <string>
#include <vector>

#include "luks.h"

#include "action.h"
#include "group.h"
#include "coset.h"
#include "ext.h"

Either<Coset,Empty> LuksChainRule( Group N, std::vector<std::set<int>> windows, std::string x, std::string y, int depth ) {
	Coset H( N, N, N->one() );
	Permutation tau = N->one();
	for( const auto& window : windows ) { // chain rule
		auto result = LuksStringIsomorphism( H.subgroup(), window, x, y, depth+1 );
		if( result.isSecond() )
			return Empty();
		H = result.getFirst();
		y = LuksStringAction( H.representative().inverse(), y/*, window*/ );
		tau = H.representative() * tau;
	}
	return Coset( N, H.subgroup(), tau );
}

Either<Coset,Empty> LuksChainRule( Group N, std::vector<std::vector<int>> windows, std::string x, std::string y, int depth ) {
	Coset H( N, N, N->one() );
	Permutation tau = N->one();
	for( const auto& window : windows ) { // chain rule
		auto result = LuksStringIsomorphism( H.subgroup(), std::set<int>( window.begin(), window.end() ), x, y, depth+1 );
		std::cerr << "chain rule intermediate: " << result << std::endl;
		if( result.isSecond() )
			return Empty();
		H = result.getFirst();
		y = LuksStringAction( H.representative().inverse(), y/*, window*/ );
		tau = H.representative() * tau;
	}
	return Coset( N, H.subgroup(), tau );
}


std::string LuksStringAction( const Permutation& sigma, std::string x, const std::vector<int>& window ) {
	std::string y = x;
	for( int i : window )
		y[i] = x[ sigma(i) ];
	return y;
} 

std::string LuksStringAction( const Permutation& sigma, std::string x, const std::set<int>& window ) {
	std::string y = x;
	for( int i : window )
		y[i] = x[ sigma(i) ];
	return y;
}

std::string LuksStringAction( const Permutation& sigma, std::string x ) {
	std::string y = x;
	for( size_t i = 0; i < x.size(); i++ )
		y[i] = x[sigma(i)];
	return y;
}

Either<Coset,Empty> LuksStringIsomorphism( Group G, std::set<int> window, std::string x, std::string y, int depth ) {
	// check if G is a subgroup of Aut(x)
	std::cerr 
		<< std::string(depth*2,' ') << "Group:  " << G->generators() << std::endl 
		<< std::string(depth*2,' ') << "Window: " << window << std::endl 
		<< std::string(depth*2,' ') << "x: " << x << std::endl 
		<< std::string(depth*2,' ') << "y: " << y << std::endl;
	bool isAut = true;
	for( const Permutation& sigma : G->generators() ) 
		if( LuksStringAction( sigma, x, window ) != x ) {
			isAut = false;
			break;
		}
	if( isAut ) {
		std::cerr << std::string(depth*2,' ') << ">is subset Aut" << std::endl;
		bool equal = true;
		for( int i : window )
			if( x[i] != y[i] ) {
				equal = false;
				break;
			}
		if( /*x == y*/ equal ) {
			std::cerr << std::string(depth*2,' ') << "return: " << G->generators() << std::endl;
			return Either<Coset,Empty>( Coset( G, G, G->one() ) );
		} else {
			std::cerr << std::string(depth*2,' ') << "return: empty" << std::endl;
			return Either<Coset,Empty>( Empty() );
		}
	}
	// G is not a subgroup of Aut(x)
	auto action = SubsetAction( G, to_vector( window ) );
	// check if G is intransitive
	if( !action.isTransitive() ) {
		std::cerr << std::string(depth*2,' ') << ">decomposition to orbits" << std::endl;
		auto r = LuksChainRule( G, action.orbits(), x, y, depth );
		std::cerr << "return: " << r << std::endl;
		return r;
	}
	// transitive case
	std::cerr << std::string(depth*2,' ') << ">decomposition to blocksystem" << std::endl;
	Action<std::set<int>> block_action = action.systemOfImprimitivity();
	std::cerr << std::string(depth*2,' ') << ">blocks: " << block_action.domain() << std::endl;
	Group N = block_action.kernel();
	std::cerr << std::string(depth*2,' ') << ">kernel: " << N->generators() << std::endl;
	Coset I( G, N, N->one() );
	std::vector<Permutation> new_generators;
	auto cosets = G->allCosets( N );
	for( auto& coset : cosets ) {
		std::cerr << std::string(depth*2,' ') << ">coset:" << coset << std::endl;
		Either<Coset,Empty> result = LuksChainRule( N, block_action.domain(), x, LuksStringAction( coset.representative().inverse(), y, window ), depth );
		if( result.isFirst() ) {
			I = result.getFirst();
			new_generators.push_back( I.representative() * coset.representative() );
		}
	}
	if( new_generators.size() == 0 ) {
		std::cerr << std::string(depth*2,' ') << "return(1): " << Empty() << std::endl;
		return Empty();
	} else /*if( new_generators.size() == 1 ) {
		std::cerr << std::string(depth*2,' ') << "return(2): " << I << std::endl;
		return I;
	} else */{
		std::cerr << new_generators.size();
		Permutation rho = new_generators.back().inverse();
		new_generators.pop_back();
		std::cerr << "a";
		for( auto& sigma : new_generators )
			sigma *= rho;
		std::cerr << "b";
		new_generators.reserve( new_generators.size() + I.subgroup()->generators().size() );
		std::cerr << I.subgroup()->generators() << std::endl;

		auto g = I.subgroup()->generators();
		new_generators.insert( new_generators.end(), g.begin(), g.end() );
				std::cerr << "b";

		Group H( new Subgroup( G, new_generators ) );
		Coset r( G, H, rho.inverse() );
		std::cerr << std::string(depth*2,' ') << "return(3): " << r << std::endl;
		return r;
	}
}

int main() {
	/*std::string x = "ababbbbb";
	std::string y = "bababbbb";
	Group S8( new SymmetricGroup(8) );
	Group G( new Subgroup( S8, {{1,2,3,0,5,4,7,6},{0,1,2,3,7,5,6,4}} ) );*/
	std::string x = "aabcaabc";
	std::string y = "baacbaac";
	Group S8( new SymmetricGroup(8) );
	Group G( new Subgroup( S8, {{3,1,2,0,4,5,6,7},{0,1,3,7,4,5,6,2},{0,1,2,4,6,5,3,7}} ) );

	auto Iso = LuksStringIsomorphism( G, {0,1,2,3,4,5,6,7}, x, y, 0 );
	std::cout << "result: " << Iso << std::endl;
	return 0; 
}