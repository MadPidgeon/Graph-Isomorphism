#pragma once 

#include "conf2.h"
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <climits>
#include <cassert>
#include "ext.h"
//#include "configuration.h"
#include "permutation.h"

struct rtype {
	int v;
	void join( rtype x ) { v += x.v; }
	rtype() { v = 0; }
};

rtype foundPartition( ColouredPartition&& P );

rtype UPCCtoBipartite( const RelationalStructure& X, double beta );

int reducePart2byColour( const ColouredBipartiteGraph& X, double alpha, std::deque<int> C1 );

rtype bipartiteSplitOrJohnson( ColouredBipartiteGraph G, double alpha );

rtype Head( ColouredBipartiteGraph G, double alpha, RelationalStructure X, RelationalStructure X1, RelationalStructure X2 );

rtype UPCCCase( ColouredBipartiteGraph G, double alpha, RelationalStructure X, RelationalStructure X1, RelationalStructure X2 );
rtype BlockDesignCase( ColouredBipartiteGraph G, double alpha, RelationalStructure X, RelationalStructure X1, RelationalStructure X2 );
rtype ImprimitiveCase( UnionFind witness, ColouredBipartiteGraph G, double alpha, RelationalStructure X, RelationalStructure X1, RelationalStructure X2 );