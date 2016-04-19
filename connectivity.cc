#include <iostream>
#include "matrix.h"
#include "unionfind.h"
#include <string>

using namespace std;

int main(int argc, char** argv) {
	cin.exceptions( std::istream::eofbit );
	int n = atoi( argv[1] );
	try {
	while( true ) {
		matrix<int> M(n,n);
		int c = 0;
		for( int i = 0; i < n; i++ ) {
			for( int j = 0; j < n; j++ ) {
				cin >> M.at( i, j );
				c = max( c, M.at( i, j ) );
			}
		}
		bool succes = true;
		for( int k = 1; k <= c; k++ ) {
			UnionFind uf( n );
			for( int i = 0; i < n; i++ ) {
				for( int j = 0; j < n; j++ ) {
					if( M.at( i, j ) == k )
						uf.cup( i, j );
				}
			}
			if( not uf.isUniform() ) {
				std::cout << k << std::endl;
				succes = false;
				break;
			}
		}
		if( succes )
			cout << M << endl;
	}} catch(...) {}
	return 0;
}