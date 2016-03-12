#include <functional>
#include <set>

#include "permutation.h"
#include "group.h"
#include "action.h"
#include "matrix.h"

Action<int> SubsetAction( Group G, std::vector<int> subset ) {
	return Action<int>( G, subset, []( const Permutation& sigma, int x ) -> int { return sigma(x); } );
}

Action<int> NaturalAction( Group G ) {
	return SubsetAction( G, G->domain() );
}

/*Action<matrix<int>> MatrixAction( Group G ) {
	throw;
}*/