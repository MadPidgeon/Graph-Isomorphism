#include <iostream>
#include <string>
#include <pair>
#include "coset.h"
#include "group.h"
#include "bruteforce.h"

std::pair<std::string,std::pair> randomStrings( int n ) {
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
	std::string y = x;
	std::random_shuffle( y.begin(), y.end() );
	return make_pair(x,y);
}

int main() {
	Group S12( new SymmetricGroup(12) );
	Group Dih12( new Subgroup( S12, { {1,2,3,4,5,6,7,8,9,10,11,0}, {6,5,4,3,2,1,0,11,10,9,8,7} } ) );
	Group A12( new Subgroup( S12, { {1,2,3,4,5,6,7,8,9,10,0,11}, {1,0,2,3,4,5,6,7,8,9,10,11} } ) );
	Group C2_6( new Subgroup( S12,  { {1,0,2,3,4,5,6,7,8,9,10,11}, {0,1,3,2,4,5,6,7,8,9,10,11}, {0,1,2,3,5,4,6,7,8,9,10,11}, {0,1,2,3,4,5,7,6,8,9,10,11}, {0,1,2,3,4,5,6,7,9,8,10,11}, {0,1,2,3,4,5,6,7,8,9,11,10} }  ) )
	for( int i = 0; i < 10; i++ ) {
		auto p = randomStrings( 12 );
		std::cout << x << std::endl << y << std::endl;
		std::cout << BruteforceIsomorphism( Dih12, p.first, p.second ) << std::endl;
		std::cout << BruteforceIsomorphism( C2_6, p.first, p.second ) << std::endl;
		std::cout << "---" << std::endl;
	}
	return 0;
}