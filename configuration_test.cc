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
	RelationalStructure conf5( vector<set<vector<int>>>({
		set<vector<int>>({
			vector<int>({0,0}),vector<int>({1,1}),vector<int>({2,2}),vector<int>({3,3})
		}),
		set<vector<int>>({
			vector<int>({0,1}),vector<int>({1,2}),vector<int>({2,3}),vector<int>({3,0})
		}),
		set<vector<int>>({
			vector<int>({1,0}),vector<int>({2,1}),vector<int>({3,2}),vector<int>({0,3})
		}),
		set<vector<int>>({
			vector<int>({1,3}),vector<int>({3,1}),
			vector<int>({2,0}),vector<int>({0,2})
		})
	}), 4 );
	RelationalStructure conf6( vector<set<vector<int>>>({
		set<vector<int>>({
			vector<int>({0,0}),vector<int>({1,1}),vector<int>({2,2}),vector<int>({3,3})
		}),
		set<vector<int>>({
			vector<int>({0,1}),vector<int>({1,2}),vector<int>({2,3}),vector<int>({3,0}),vector<int>({1,0}),vector<int>({2,1}),vector<int>({3,2}),vector<int>({0,3})
		}),
		set<vector<int>>({
			vector<int>({1,3}),vector<int>({3,1}),
			vector<int>({2,0}),vector<int>({0,2})
		})
	}), 4 );
	RelationalStructure conf7( vector<set<vector<int>>>({
		set<vector<int>>({
			vector<int>({0,0,0}),vector<int>({1,1,1}),vector<int>({2,2,2}),vector<int>({3,3,3})
		}),
		set<vector<int>>({
			vector<int>({0,1,2}),vector<int>({1,2,3}),vector<int>({2,3,0}),vector<int>({3,0,1})
		}),
		set<vector<int>>({
			vector<int>({2,1,0}),vector<int>({3,2,1}),vector<int>({0,3,2}),vector<int>({1,0,3})
		}),
		set<vector<int>>({
			vector<int>({0,1,3}),vector<int>({0,3,1}),vector<int>({0,2,1}),vector<int>({0,2,3}),
			vector<int>({1,2,0}),vector<int>({1,0,2}),vector<int>({1,3,0}),vector<int>({1,3,2}),
			vector<int>({2,1,3}),vector<int>({2,3,1}),vector<int>({2,0,1}),vector<int>({2,0,3}),
			vector<int>({3,2,0}),vector<int>({1,0,2}),vector<int>({1,3,0}),vector<int>({1,3,2}),
		}),
		set<vector<int>>({
			vector<int>({0,0,1}),vector<int>({0,0,2}),vector<int>({0,0,3}),
			vector<int>({1,1,0}),vector<int>({1,1,2}),vector<int>({1,1,3}),
			vector<int>({2,2,0}),vector<int>({2,2,1}),vector<int>({2,2,3}),
			vector<int>({3,3,0}),vector<int>({3,3,1}),vector<int>({3,3,2})
		}),
		set<vector<int>>({
			vector<int>({0,1,1}),vector<int>({0,2,2}),vector<int>({0,3,3}),
			vector<int>({1,0,0}),vector<int>({1,2,2}),vector<int>({1,3,3}),
			vector<int>({2,0,0}),vector<int>({2,1,1}),vector<int>({2,3,3}),
			vector<int>({3,0,0}),vector<int>({3,1,1}),vector<int>({3,2,2})
		}),
		set<vector<int>>({
			vector<int>({1,0,1}),vector<int>({2,0,2}),vector<int>({3,0,3}),
			vector<int>({0,1,0}),vector<int>({2,1,2}),vector<int>({3,1,3}),
			vector<int>({0,2,0}),vector<int>({1,2,1}),vector<int>({3,2,3}),
			vector<int>({0,3,0}),vector<int>({1,3,1}),vector<int>({2,3,2})
		}),
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
	conf5.WeisfellerLeman();
	std::cout << conf5.relations() << std::endl;
	std::cout << "------------------------------" << std::endl;
	conf6.WeisfellerLeman();
	std::cout << conf6.relations() << std::endl;
	std::cout << "------------------------------" << std::endl;
	vector<set<vector<int>>> R8(3);
	for( auto t : all_tuples( 8, 2 ) )
		if( t[0] == t[1] )
			R8[0].insert( t );
		else if( (t[0] & 3) == (t[1] & 3) )
			R8[1].insert( t );
		else
			R8[2].insert( t );
	RelationalStructure conf8( R8, 8 );
	conf8.WeisfellerLeman();
	std::cout << conf8.relations() << std::endl;

	std::cout << "------------------------------" << std::endl;
	vector<set<vector<int>>> R9(4);
	for( auto t : all_tuples( 5, 2 ) )
		if( t[0] == t[1] )
			R9[0].insert( t );
		else if( t[0] == (t[1]+1)%5 )
			R9[1].insert( t );
		else if( t[0] == (t[1]+4)%5 )
			R9[2].insert( t );
		else
			R9[3].insert( t );
	RelationalStructure conf9( R9, 5 );
	conf9.WeisfellerLeman();
	std::cout << conf9.relations() << std::endl;

	std::cout << "------------------------------" << std::endl;
	vector<set<int>> adj10 = {{3, 5, 6, 9, 10}, {4, 7, 8, 11, 12}, {1, 7, 8, 9, 10}, {2, 5, 6, 11, 12}, {1, 4, 6, 9, 11}, {1, 4, 5, 10, 12}, {2, 3, 8, 9, 11}, {2, 3, 7, 10, 12}, {1, 3, 5, 7, 11}, {1, 3, 6, 8, 12}, {2, 4, 5, 7, 9}, {2, 4, 6, 8, 10}};
	vector<set<vector<int>>> R10(3);
	for( auto t : all_tuples( 12, 2 ) )
		if( t[0] == t[1] )
			R10[0].insert( t );
		else if( adj10[t[0]].count( t[1]+1 ) )
			R10[1].insert( t );
		else
			R10[2].insert( t );
	RelationalStructure conf10( R10, 12 );
	conf10.WeisfellerLeman();
	std::cout << conf10.relations() << std::endl;

	std::cout << "------------------------------" << std::endl;
	matrix<int> M12({{0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1},
{0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,1},
{0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,1},
{0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0},
{1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0},
{0,1,0,0,0,0,0,0,1,0,1,1,0,1,0,0},
{0,0,1,0,0,0,0,0,1,1,0,1,0,0,1,0},
{0,0,0,1,0,0,0,0,1,1,1,0,0,0,0,1},
{1,0,0,0,0,1,1,1,0,0,0,0,1,0,0,0},
{0,1,0,0,1,0,1,1,0,0,0,0,0,1,0,0},
{0,0,1,0,1,1,0,1,0,0,0,0,0,0,1,0},
{0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1},
{0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0},
{1,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0},
{1,1,0,1,0,0,1,0,0,0,1,0,0,0,0,0},
{1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,0}});
	vector<set<vector<int>>> R12(3);
	for( auto t : all_tuples( 16, 2 ) ) {
		if( t[0] == t[1] )
			R12[0].insert({t[0],t[1]});
		else
			R12[ M12.at( t[0],t[1] )+1 ].insert({t[0],t[1]}); 
	}
	RelationalStructure conf12( R12, 16 );
	conf12.WeisfellerLeman();
	std::cout << conf12.relations() << std::endl;
	std::cout << "---" << std::endl;
	conf12.individualize({0});
	std::cout << conf12.relations() << std::endl;
	std::cout << "---" << std::endl;
	conf12.WeisfellerLeman();
	std::cout << conf12.relations() << std::endl;

	std::cout << "--------------------------" << std::endl;
	// chang
	std::cout << "chang" << std::endl;
	matrix<int> M15({{0,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{2,0,2,2,2,2,2,2,1,1,1,1,1,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1},
{2,2,0,1,2,2,2,1,2,2,1,1,1,1,2,1,2,1,2,1,2,2,1,1,1,1,1,1},
{2,2,1,0,2,2,1,2,1,1,2,2,1,2,1,2,1,1,1,2,1,1,2,2,1,1,1,1},
{2,2,2,2,0,2,1,1,1,1,2,2,1,1,2,1,2,1,1,1,2,1,2,1,1,1,2,1},
{2,2,2,2,2,0,1,1,1,2,1,2,1,1,1,2,2,1,1,1,1,2,1,2,1,1,1,2},
{2,2,2,1,1,1,0,2,2,2,1,1,2,2,2,1,1,2,2,1,1,1,1,1,1,2,1,1},
{2,2,1,2,1,1,2,0,2,1,2,1,2,2,1,2,1,2,1,2,1,1,1,1,2,1,1,1},
{2,1,2,1,1,1,2,2,0,2,2,1,2,1,1,1,1,1,2,2,2,2,1,1,2,1,1,1},
{2,1,2,1,1,2,2,1,2,0,1,2,2,1,1,1,1,1,2,1,1,2,1,2,1,2,1,2},
{2,1,1,2,2,1,1,2,2,1,0,2,2,1,1,1,1,1,1,2,2,1,2,1,2,1,2,1},
{2,1,1,2,2,2,1,1,1,2,2,0,2,1,1,1,1,1,1,1,1,1,2,2,1,2,2,2},
{2,1,1,1,1,1,2,2,2,2,2,2,0,1,1,1,1,2,1,1,1,1,1,1,2,2,2,2},
{1,2,1,2,1,1,2,2,1,1,1,1,1,0,2,2,1,2,2,2,1,1,2,2,1,2,1,1},
{1,2,2,1,2,1,2,1,1,1,1,1,1,2,0,1,2,2,2,1,2,1,2,1,1,2,2,1},
{1,2,1,2,1,2,1,2,1,1,1,1,1,2,1,0,2,2,1,2,1,2,1,2,2,1,1,2},
{1,2,2,1,2,2,1,1,1,1,1,1,1,1,2,2,0,2,1,1,2,2,1,1,2,1,2,2},
{1,2,1,1,1,1,2,2,1,1,1,1,2,2,2,2,2,0,1,1,1,1,1,1,2,2,2,2},
{1,1,2,1,1,1,2,1,2,2,1,1,1,2,2,1,1,1,0,2,2,2,2,2,1,2,1,1},
{1,1,1,2,1,1,1,2,2,1,2,1,1,2,1,2,1,1,2,0,2,2,2,2,2,1,1,1},
{1,1,2,1,2,1,1,1,2,1,2,1,1,1,2,1,2,1,2,2,0,2,2,1,2,1,2,1},
{1,1,2,1,1,2,1,1,2,2,1,1,1,1,1,2,2,1,2,2,2,0,1,2,2,1,1,2},
{1,1,1,2,2,1,1,1,1,1,2,2,1,2,2,1,1,1,2,2,2,1,0,2,1,2,2,1},
{1,1,1,2,1,2,1,1,1,2,1,2,1,2,1,2,1,1,2,2,1,2,2,0,1,2,1,2},
{1,1,1,1,1,1,1,2,2,1,2,1,2,1,1,2,2,2,1,2,2,2,1,1,0,1,2,2},
{1,1,1,1,1,1,2,1,1,2,1,2,2,2,2,1,1,2,2,1,1,1,2,2,1,0,2,2},
{1,1,1,1,2,1,1,1,1,1,2,2,2,1,2,1,2,2,1,1,2,1,2,1,2,2,0,2},
{1,1,1,1,1,2,1,1,1,2,1,2,2,1,1,2,2,2,1,1,1,2,1,2,2,2,2,0}});
	vector<set<vector<int>>> R15(3);
	for( auto t : all_tuples( 28, 2 ) )
		R15[ M15.at( t[0], t[1] ) ].insert({t[0],t[1]});
	RelationalStructure conf15( R15, 28 );
	conf15.individualize({4});
	conf15.WeisfellerLeman();
	std::cout << conf15.relations() << std::endl;

	std::cout << "--------------------------" << std::endl;
	vector<set<vector<int>>> R14(6);
	for( auto t : all_tuples( 6, 3 ) ) {
		if( t[0] == t[1] and t[1] == t[2] )
			R14[0].insert({t[0],t[1],t[2]});
		else if( t[0] == t[1] )
			R14[1].insert({t[0],t[1],t[2]});
		else if( t[1] == t[2] )
			R14[2].insert({t[0],t[1],t[2]});
		else if( t[0] == t[2] )
			R14[3].insert({t[0],t[1],t[2]});
		else if( ( (t[0]+1)%6 == t[1] or (t[0]+5)%6 == t[1] or (t[0]+3)%6 == t[1] ) and
				 ( (t[0]+1)%6 == t[2] or (t[0]+5)%6 == t[2] or (t[0]+3)%6 == t[2] ) )
			R14[4].insert({t[0],t[1],t[2]});
		else
			R14[5].insert({t[0],t[1],t[2]});
	}
	RelationalStructure conf14( R14, 6 );
	conf14.WeisfellerLeman();
	std::cout << conf14.relations() << std::endl;
	std::cout << "---" << std::endl;
	conf14.individualize({0});
	conf14.WeisfellerLeman();
	std::cout << conf14.relations() << std::endl;
	std::cout << "---" << std::endl;

	std::cout << "------------------------------" << std::endl;
	// https://caagt.ugent.be/tca/
	matrix<int> M11({{0,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3},
{1,0,1,1,1,1,2,2,2,2,2,2,3,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,1,2,2,2},
{1,1,0,1,1,2,1,2,2,2,2,3,2,1,2,2,2,2,3,1,1,1,1,2,2,2,3,3,1,2,2,2,1,2,2},
{1,1,1,0,3,2,2,1,1,2,2,2,2,2,1,1,2,2,2,1,1,2,2,3,3,3,1,2,2,1,2,2,2,1,2},
{1,1,1,3,0,2,2,2,2,1,1,2,2,2,2,2,1,1,2,2,2,1,1,3,3,3,2,1,2,2,1,2,2,2,1},
{1,1,2,2,2,0,2,1,2,1,3,1,2,2,1,2,1,3,1,3,3,2,2,1,1,2,2,2,1,2,2,2,1,2,2},
{1,2,1,2,2,2,0,2,1,3,1,2,1,2,3,3,2,2,1,1,2,1,3,1,2,1,2,2,1,2,2,1,2,2,2},
{1,2,2,1,2,1,2,0,3,2,1,1,2,3,1,2,3,2,2,2,2,3,1,2,2,1,1,2,1,2,1,2,2,1,2},
{1,2,2,1,2,2,1,3,0,1,2,2,1,3,2,2,3,1,1,1,2,3,2,2,1,2,2,1,2,1,2,2,2,1,2},
{1,2,2,2,1,1,3,2,1,0,2,2,1,2,3,3,1,2,2,2,1,2,3,2,1,2,1,1,2,2,1,2,1,2,2}
,{1,2,2,2,1,3,1,1,2,2,0,1,2,2,2,1,2,3,2,3,3,1,2,2,2,1,2,1,2,1,1,1,2,2,2},
{1,2,3,2,2,1,2,1,2,2,1,0,1,1,2,1,2,2,3,2,2,2,1,1,1,2,3,3,2,1,2,2,2,2,1},
{1,3,2,2,2,2,1,2,1,1,2,1,0,1,2,2,2,1,2,2,1,2,2,1,2,1,1,2,3,3,3,2,2,2,1},
{2,1,1,2,2,2,2,3,3,2,2,1,1,0,2,1,3,1,2,2,1,3,1,2,1,1,2,2,2,2,2,1,1,2,2},
{2,1,2,1,2,1,3,1,2,3,2,2,2,2,0,3,2,1,1,2,2,1,3,2,2,1,1,2,2,1,2,2,1,2,1},
{2,1,2,1,2,2,3,2,2,3,1,1,2,1,3,0,1,2,2,2,1,2,3,1,2,2,2,1,2,1,2,1,2,1,2},
{2,1,2,2,1,1,2,3,3,1,2,2,2,3,2,1,0,2,2,1,2,3,2,1,2,2,1,1,1,2,2,1,2,2,1},
{2,1,2,2,1,3,2,2,1,2,3,2,1,1,1,2,2,0,1,3,3,2,1,2,2,1,2,1,2,2,2,2,2,1,1},
{2,1,3,2,2,1,1,2,1,2,2,3,2,2,1,2,2,1,0,2,2,1,2,1,1,2,3,3,2,2,1,1,2,1,2},
{2,2,1,1,2,3,1,2,1,2,3,2,2,2,2,2,1,3,2,0,3,2,1,2,1,2,1,2,1,1,2,1,2,2,1},
{2,2,1,1,2,3,2,2,2,1,3,2,1,1,2,1,2,3,2,3,0,1,2,1,2,2,1,2,2,2,1,2,1,1,2},
{2,2,1,2,1,2,1,3,3,2,1,2,2,3,1,2,3,2,1,2,1,0,2,1,2,2,2,2,2,1,1,2,1,2,1},
{2,2,1,2,1,2,3,1,2,3,2,1,2,1,3,3,2,1,2,1,2,2,0,2,1,2,2,2,1,2,1,2,2,1,1},
{2,2,2,3,3,1,1,2,2,2,2,1,1,2,2,1,1,2,1,2,1,1,2,0,3,3,2,2,1,2,2,2,2,1,1},
{2,2,2,3,3,1,2,2,1,1,2,1,2,1,2,2,2,2,1,1,2,2,1,3,0,3,2,2,2,1,1,1,1,2,2},
{2,2,2,3,3,2,1,1,2,2,1,2,1,1,1,2,2,1,2,2,2,2,2,3,3,0,1,1,1,2,2,1,1,2,2},
{2,2,3,1,2,2,2,1,2,1,2,3,1,2,1,2,1,2,3,1,1,2,2,2,2,1,0,3,2,2,1,1,2,2,1},
{2,2,3,2,1,2,2,2,1,1,1,3,2,2,2,1,1,1,3,2,2,2,2,2,2,1,3,0,1,1,2,2,1,1,2},
{2,3,1,2,2,1,1,1,2,2,2,2,3,2,2,2,1,2,2,1,2,2,1,1,2,1,2,1,0,3,3,2,1,1,2},
{2,3,2,1,2,2,2,2,1,2,1,1,3,2,1,1,2,2,2,1,2,1,2,2,1,2,2,1,3,0,3,2,1,2,1},
{2,3,2,2,1,2,2,1,2,1,1,2,3,2,2,2,2,2,1,2,1,1,1,2,1,2,1,2,3,3,0,1,2,1,2},
{3,1,2,2,2,2,1,2,2,2,1,2,2,1,2,1,1,2,1,1,2,2,2,2,1,1,1,2,2,2,1,0,3,3,3},
{3,2,1,2,2,1,2,2,2,1,2,2,2,1,1,2,2,2,2,2,1,1,2,2,1,1,2,1,1,1,2,3,0,3,3},
{3,2,2,1,2,2,2,1,1,2,2,2,2,2,2,1,2,1,1,2,1,2,1,1,2,2,2,1,1,2,1,3,3,0,3},
{3,2,2,2,1,2,2,2,2,2,2,1,1,2,1,2,1,1,2,1,2,1,1,1,2,2,1,2,2,1,2,3,3,3,0}});
	vector<set<vector<int>>> R11(4);
	for( auto t : all_tuples( 35, 2 ) ) {
		R11[ M11.at( t[0],t[1] ) ].insert({t[0],t[1]}); 
	}
	RelationalStructure conf11( R11, 35 );
	conf11.WeisfellerLeman();
	std::cout << conf11.isUniprimitive() << std::endl;

	std::cout << "------------------------------" << std::endl;
	matrix<int> M13({{0,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0},
{1,0,1,1,1,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
{1,1,0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0},
{1,1,1,0,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0},
{1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0},
{1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0},
{0,1,0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
{0,0,1,0,0,0,1,1,0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0},
{0,0,0,1,0,0,1,1,1,0,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0},
{0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0},
{0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
{1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0},
{0,1,0,0,0,0,0,1,0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0},
{0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0},
{0,0,0,1,0,0,0,0,0,1,0,0,1,1,1,0,1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0},
{0,0,0,0,1,0,0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0},
{0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0},
{0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,1,0,0,0,0},
{0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0},
{0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,1,1,0,1,1,0,0,0,1,0,0,0,0,0,1,0,0},
{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,0,1,0},
{0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1},
{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
{0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0},
{0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,1,1,1,0,0,1,0,0,0},
{0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,1,1,0,1,1,0,0,0,1,0,0},
{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,1,1,1,1,0,1,0,0,0,0,1,0},
{0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1},
{1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1},
{0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,1,1,1,1},
{0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,1,1,1},
{0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,1,1,0,1,1},
{0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,1,1,1,1,0,1},
{0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,0}});
	vector<set<vector<int>>> R13(3);
	for( auto t : all_tuples( 36, 2 ) ) {
		if( t[0] == t[1] )
			R13[0].insert({t[0],t[1]});
		else
			R13[ M13.at( t[0],t[1] )+1 ].insert({t[0],t[1]}); 
	}
	RelationalStructure conf13( R13, 36 );
	conf13.individualize({0});
	conf13.WeisfellerLeman();
	std::cout << conf13.relations() << std::endl;
	std::cout << conf13.isUniprimitive() << std::endl;
	std::cout << "---" << std::endl;
	conf13.individualize({0});
	std::cout << conf13.relations() << std::endl;
	std::cout << "---" << std::endl;
	conf13.WeisfellerLeman();
	std::cout << conf13.relations() << std::endl;
	

	/*int n = 12;
	vector<set<vector<int>>> R(4);
	for( vector<int> tup : all_tuples( n, 3 ) ) {
		if( tup[0] == tup[1] && tup[1] == tup[2] )
			R[0].insert( tup );
		else if( tup[0] == tup[1] || tup[1] == tup[2] || tup[2] == tup[0] )
			R[1].insert( tup );
		else for( int i = 0; i < 3; i++) {
			int j = (i + 1) % 3;
			int k = (i + 2) % 3;
			if( (tup[i] % 6) == (tup[j] % 6 ) ) {
				R[2].insert( tup );
				goto forbah;
			}
		}
		R[3].insert( tup );
forbah:;
	}
	RelationalStructure conf7( R, n );
	conf7.WeisfellerLeman();
	std::cout << conf7.relations() << std::endl;
	std::cout << "------------------------------" << std::endl;*/


	/*Graph G1( 7 );
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
	randomIsomorphismProblem( 10 );*/
}