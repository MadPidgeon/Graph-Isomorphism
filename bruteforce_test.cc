#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <ctime>
#include "coset.h"
#include "group.h"
#include "bruteforce.h"

std::pair<std::string,std::string> randomStrings( int n ) {
	std::vector<int> cvalues;
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
	std::string y = x;
	std::random_shuffle( y.begin(), y.end() );
	return make_pair(x,y);
}

int main() {
	Group S10( new SymmetricGroup(10) );
	Group Dih10( new Subgroup( S10, { {1,2,3,4,5,6,7,8,9,0}, {5,4,3,2,1,0,9,8,7,6} } ) );
	Group A10( new Subgroup( S10, { {1,2,3,4,5,6,7,8,0,9}, {0,1,2,3,4,5,6,8,9,7} } ) );
	Group C2_5( new Subgroup( S10,  { {1,0,2,3,4,5,6,7,8,9}, {0,1,3,2,4,5,6,7,8,9}, {0,1,2,3,5,4,6,7,8,9}, {0,1,2,3,4,5,7,6,8,9}, {0,1,2,3,4,5,6,7,9,8}} ) );
	Group G( new Subgroup( S10,  { {1,0,2,3,4,5,6,7,8,9}, {0,1,3,2,4,5,6,7,8,9}, {0,1,2,3,5,4,6,7,8,9}, {0,1,2,3,4,5,7,6,8,9}, {0,1,2,3,4,5,6,7,9,8}, {1,2,3,4,5,6,7,8,9,0} } ) );
	srand( time( 0 ) );
	for( int i = 0; i < 10; i++ ) {
		auto p = randomStrings( 10 );
		std::cout << p.first << std::endl << p.second << std::endl;
		std::cout << BruteforceIsomorphism( Dih10, p.first, p.second ) << std::endl;
		std::cout << BruteforceIsomorphism( C2_5, p.first, p.second ) << std::endl;
		std::cout << BruteforceIsomorphism( G, p.first, p.second ) << std::endl;
		std::cout << BruteforceIsomorphism( A10, p.first, p.second ) << std::endl;
		std::cout << BruteforceIsomorphism( S10, p.first, p.second ) << std::endl;
		std::cout << "---" << std::endl;
	}
	return 0;
}