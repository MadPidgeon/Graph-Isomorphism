#include <iostream>
#include <string>
#include "ext.h"
#include "configuration.h"

using namespace std;

int main() {
	string line;
	int n = -1;
	cin.exceptions( std::istream::eofbit );
	getline( cin, line );
	try{
	while(true) {
		getline( cin, line );
		getline( cin, line );
		n = line.size();
		std::vector<std::set<std::vector<int>>> R(3);
		for( int i = 0; i < n; i++ ) {
			for( int j = 0; j < n; j++ )
				if( i == j )
					R[0].insert({i,j});
				else if( line[j] == '1' )
					R[1].insert({i,j});
				else
					R[2].insert({i,j});
			getline( cin, line );
		}
		RelationalStructure X( R, n );
		X.WeisfellerLeman();
		if( X.isUniprimitive() ) {
			std::cout << X.relations() << std::endl;
		}
	}} catch(...){}
	return 0;
}