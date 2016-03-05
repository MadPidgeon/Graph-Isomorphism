#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "group.h"
#include "ext.h"
#include "configuration.h"
#include "action.h"

using std::vector;
using std::set;

std::string randomString( int n ) {
	vector<int> cvalues;
	int c = 0;
	int total = 0;
	while( total < 4*n ) {
		cvalues.push_back( (rand()%n)+1 );
		total += cvalues.back();
		c++;
	}
	std::string x( total, 'a' );
	int j = 0;
	for( int i = 0; i < c; i++ )
		for( int k = 0; k < cvalues[i]; k++ )
			x[j++] += i;  
	std::random_shuffle( x.begin(), x.end() );
	x.resize( n );
	return x;
}

Group randomGroup( int n ) {
	int d = n/4;
	int r = n*n*n;
	Group S( new SymmetricGroup( n ) );
	return Group( new Subgroup( S, {S->generators()[0]} ) );
	/*auto generators = S->generators();
	std::vector<Permutation> new_generators;
	for( int i = 0; i < d; i++ ) {
		Permutation alpha = S->one();
		int k = (rand() % r) + 100;
		for( int j = 0; j < k ; j++ )
			alpha *= (generators[ 0 ]^( rand() % 10 )) * (generators[ 1 ]^( rand() % 10 ));
		new_generators.emplace_back( std::move( alpha ) );
	}*/
	//return Group( new Subgroup( S, new_generators ) );
}

void randomIsomorphismProblem( int n ) {
	auto x = randomString( n );
	std::cout << "string: " << x << std::endl;
	auto G = randomGroup( n );
	std::cout << "group: " << G->generators() << std::endl;
	std::cout << "order: " << G->order() << std::endl;
}

int main() {
	RelationalStructure conf1( vector<set<vector<int>>>({
		set<vector<int>>({
			vector<int>({0,0})
		})
	}), 1 );
	RelationalStructure conf2( vector<set<vector<int>>>({
		set<vector<int>>({
			vector<int>({0,0})
		}),
		set<vector<int>>({
			vector<int>({1,0})
		}),
		set<vector<int>>({
			vector<int>({0,1})
		}),
		set<vector<int>>({
			vector<int>({1,1})
		})
	}), 2 );
	RelationalStructure conf3( vector<set<vector<int>>>({
		set<vector<int>>({
			vector<int>({0,0}),vector<int>({1,1}),vector<int>({2,2}),vector<int>({3,3})
		}),
		set<vector<int>>({
			vector<int>({0,1}),vector<int>({2,3}),vector<int>({1,0}),vector<int>({3,2})
		}),
		set<vector<int>>({
			vector<int>({0,2}),vector<int>({0,3}),vector<int>({1,2}),vector<int>({1,3}),
			vector<int>({2,0}),vector<int>({3,0}),vector<int>({2,1}),vector<int>({3,1})
		})
	}), 4 );
	RelationalStructure conf4( vector<set<vector<int>>>({
		set<vector<int>>({
			vector<int>({0,0}),vector<int>({1,1}),vector<int>({2,2}),vector<int>({3,3})
		}),
		set<vector<int>>({
			vector<int>({0,1}),vector<int>({1,0})
		}),
		set<vector<int>>({
			vector<int>({2,0}),vector<int>({0,2}),vector<int>({1,2}),vector<int>({2,1}),
		}),
		set<vector<int>>({
			vector<int>({0,3}),vector<int>({3,2}),vector<int>({1,3}),
			vector<int>({3,0}),vector<int>({2,3}),vector<int>({3,1})
		})
	}), 4 );


	conf1.WeisfellerLeman();
	std::cout << conf1.relations() << std::endl;
	std::cout << "------------------------------" << std::endl;
	conf2.WeisfellerLeman();
	std::cout << conf2.relations() << std::endl;
	std::cout << "------------------------------" << std::endl;
	conf3.WeisfellerLeman();
	std::cout << conf3.relations() << std::endl;
	std::cout << "------------------------------" << std::endl;
	conf4.WeisfellerLeman();
	std::cout << conf4.relations() << std::endl;
	std::cout << "------------------------------" << std::endl;
	Graph G1( 7 );
	std::vector<std::vector<int>> edges1( {{0,3},{0,4},{0,5},{1,4},{1,5},{1,6},{2,5},{2,6},{3,4},{3,6},{4,6},{5,6}} );
	for( auto& e : edges1 )
		G1.edge( e[0], e[1] ) = G1.edge( e[1], e[0] ) = true;
	std::cout << G1.adjacencyMatrix() << std::endl;
	std::cout << "----" << std::endl;
	RelationalStructure X1( G1, 2 );
	std::cout << X1.relations() << std::endl;
	X1.WeisfellerLeman();
	std::cout << "----" << std::endl;
	std::cout << X1.relations() << std::endl;
	std::cout << "----" << std::endl;
	Group S7( new SymmetricGroup( 7 ) );
	Permutation sigma({4,2,6,1,5,0,3});
	Group H( new Subgroup( S7, {sigma} ) );
	auto A = NaturalAction( H ).tupleAction( 2 );
	Graph G2( 7 );
	std::vector<std::vector<int>> edges2;
	for( auto& e : edges1 )
		edges2.push_back( A( sigma, e ) );
	for( auto& e : edges2 )
		G2.edge( e[0], e[1] ) = G2.edge( e[1], e[0] ) = true;
	std::cout << G2.adjacencyMatrix() << std::endl;
	std::cout << "----" << std::endl;
	RelationalStructure X2( G2, 2 );
	std::cout << X2.relations() << std::endl;
	X2.WeisfellerLeman();
	std::cout << "----" << std::endl;
	std::cout << X2.relations() << std::endl;
	std::cout << "------------------------------" << std::endl;
	srand(time(0));
	randomIsomorphismProblem( 10 );
}