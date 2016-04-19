#include "align.h"

Permutation pullback( Permutation sigma_bar, const inv_gamma_t& inv_gamma, const gamma_t& gamma ) {
	int n = sigma_bar.degree();
	matrix<bool> M( n, n );
	for( int i : range(0,n) ) {
		int j = sigma_bar(i);
		for( int x : gamma[i] )
			for( int y : gamma[j] )
				M.at( x, y ) = true;
	}
	std::vector<int> matching = bipartiteMatching( M );
	if( std::find( matching.begin(), matching.end(), -1 ) != matching.end() )
		throw;
	return Permutation( std::move( matching ) );
}
