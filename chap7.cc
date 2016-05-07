#include "chap7.h"

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

rtype foundPartition( ColouredPartition&& P ) {
	std::cout << P.getMapping() << std::endl;
	return rtype();
}

// ********************************************************************************************** //
// OPTIMALISATIES KOMEN LATER
// ********************************************************************************************** //


rtype UPCCtoBipartite( const RelationalStructure& X, double beta ) {
	std::cout << "UPCCtoBipartite" << std::endl;
	assert( X.isUPCC() );
	const auto& Omega = X.domain();
	int n = Omega.size();
	int r = X.relations().size();
	rtype result;
	for( auto x : Omega ) {
		// find the C[i]
		std::deque<std::deque<int>> C( r );
		for( int y : Omega )
			C.at( X.edgeColour( x, y ) ).push_back( y );
		std::cout << C << std::endl;
		// check whether all C[i] are small
		int k = -1;
		for( int c : X.relations() ) {
			if( C[c].size() > beta*n ) {
				k = c;
				break;
			}
		}
		// if so, return coloured partition, stop.
		if( k == -1 ) {
			auto r = foundPartition( ColouredPartition( std::move( C ) ) );
			result.join( r );
			continue;
		}
		std::cout << "k=" << k << std::endl;
		// else, we have a large partition k
		// find R_i
		int i = INT_MAX;
		for( auto y : Omega ) {
			int c = X.edgeColour( x, y );
			if( c != k and c != 0 )
				i = std::min( i, c );
		}
		assert( i != INT_MAX );
		std::cout << "i=" << i << std::endl;
		// find R_j
		int j = INT_MAX;
		for( int z : C[k] ) {
			for( int y : C[i] ) {
				int c = X.edgeColour( z, y );
				if( c != k and c != 0 )
					j = std::min( j, c );
			}
		}
		assert( j != INT_MAX );
		std::cout << "j=" << j << std::endl;
		// find G
		ColouredBipartiteGraph::parameter E;
		for( auto p : X.relation( j ) )
			if( std::find( C[k].begin(), C[k].end(), p[0] ) != C[k].end() 
					and std::find( C[i].begin(), C[i].end(), p[1] ) != C[i].end() )
				E[p[0]].push_back(p[1]);
		ColouredBipartiteGraph G( C[k], C[i], E ); // = R[j] cap C[k] times C[i]
		//std::cout << "G=" << G << std::endl;
		int dk = C[k].size();
		double alpha = beta*n / dk;
		// find twins
		std::deque<std::deque<int>> T = G.twins( ColouredBipartiteGraph::LEFT ); // TODO: ordered by size
		std::cout << "twins=" << T << std::endl;
		int defect = dk - T[0].size();
		std::cout << "defect=" << defect << std::endl;
		std::cout << "beta n=" << beta*n << std::endl;
		// if small defect
		if( defect <= beta*n ) {
			std::cout << "small defect" << std::endl;
			int twins_count = T.size();
			std::deque<int> twin_complement;
			while( T[twins_count-1].size() == 1 )
				twin_complement.push_back( T[twins_count-1].front() );
			T.erase( T.begin() + twins_count, T.end() );
			std::deque<int> not_Ck;
			for( int c : X.relations() ) {
				if( c != k ) {
					not_Ck.insert( not_Ck.end(), C[c].begin(), C[c].end() );
					C[c].clear();
				}
			}
			auto r = foundPartition( ColouredPartition({ T, { twin_complement }, { not_Ck } }) );
			result.join( r );
		} else { // if large defect
			std::cout << "large defect" << std::endl;
			auto r = bipartiteSplitOrJohnson( G, alpha );
			result.join( r );
		}
	}
	return result;
}


int reducePart2byColour( const ColouredBipartiteGraph& X, double alpha, std::deque<int> C1 ) {
	auto Y1 = X.substructure( X.vertices( ColouredBipartiteGraph::LEFT ), C1 );
	if( Y1.symmetryDefect( ColouredBipartiteGraph::LEFT ) >= 1-alpha )
		return 0;
	else
		return 1;
}

rtype bipartiteSplitOrJohnson( ColouredBipartiteGraph G, double alpha ) {
	std::cout << "bipartiteSplitOrJohnson(" << alpha << ")" << std::endl;
	const int C0 = 7; // an non-alpha block needs to be of size >= 3
	auto V1 = G.vertices( ColouredBipartiteGraph::LEFT );
	auto V2 = G.vertices( ColouredBipartiteGraph::RIGHT );
	int n1 = V1.size();
	int n2 = V2.size();
	int n = n1 + n2;
	// 1.
	if( n1 <= C0 ) {
		std::cout << "n1 is negligible" << std::endl;
		/*ColouredPartition cp( std::move( G ) );
		int c = 0;
		int i = 0;
		int b = (1-alpha) * n1;
		while( c < b and i < n ) {
			if( G.vertex( i ) == 0 ) {
				cp.individualize( i );
				c++;
			}
			i++;
		}
		return cp;*/
		rtype result;
		for( auto& perm : all_permutations( n1 ) ) {
			ColouredSet cs( G );
			cs.individualise( perm.getArrayNotation() );
			result.join( foundPartition( ColouredPartition( cs ) ) );
		}
		return result;
	}
	// 2.
	RelationalStructure X( G );
	if( /*n2 <= n1*/ false ) {
		std::cout << "polylog bullshit" << std::endl;
		throw;
		#warning "Split or Johnson requires quasipolynomial function"
		rtype result;
		for( auto& perm : all_permutations( n2 ) ) {
			std::vector<int> q = perm.getArrayNotation();
			for( int& x : q )
				x = V2[x];
			std::cout << q << std::endl;
			RelationalStructure Y = X;

			//std::cout << "lolwat" << std::endl;
			Y.individualise( q );
			//std::cout << "\033[32m";
			Y.refine();
			//std::cout << "\033[0m";
			//std::cout << "Y=" << Y << std::endl;

			//std::cout << "lolwat" << std::endl;

			result.join( foundPartition( (ColouredPartition)(ColouredSet) Y ) );
		}
		return result;
	}
	// 3.
	X.refine();
	std::cout << "X=" << X << std::endl;
	// 4.
	int sup_alpha_colour = X.getNonAlphaPartition( alpha );
	if( sup_alpha_colour == -1 )
		return foundPartition( (ColouredPartition)(ColouredSet) X );
	// 5.
	auto W1 = X.relation( sup_alpha_colour ).vertices();
	alpha = alpha * n1 / W1.size();
	G = G.substructure( W1, V2 );
	X = RelationalStructure( G );
	X.refine();
	//std::cout << "G=" << G << std::endl;
	//std::cin.get();
	//std::cout << "X'=" << X << std::endl;
	//std::cin.get();
	// 6.
	std::deque<std::deque<int>> twins = G.twins( ColouredBipartiteGraph::LEFT );
	std::cout << twins << std::endl;
	if( twins.size() < W1.size() ) {
		std::cout << "enough twins" << std::endl;
		std::deque<int> complement;
		std::set_difference( V1.begin(), V1.end(), W1.begin(), W1.end(), std::back_inserter( complement ) );
		return foundPartition( ColouredPartition({ twins, std::deque<std::deque<int>>({ complement }) }) );
	}
	// 7.
	auto X1 = X.skeletalSubstructure( 2, W1 );
	auto X2 = X.skeletalSubstructure( 2, V2 );
	if( not X2.isHomogeneous() ) {
		std::cout << "not homogeneous" << std::endl;
		std::deque<int> U = X2.relation(0).vertices();
		int u = reducePart2byColour( G, alpha, U );
		if( u != 0 ) {
			std::deque<int> Uc;
			std::cout << "U=" << U << std::endl;
			std::set_difference( V2.begin(), V2.end(), U.begin(), U.end(), std::back_inserter( Uc ) );
			std::cout << "U^c=" << Uc << std::endl;
			U = std::move( Uc );
		}
		G = G.substructure( W1, U );
		X = RelationalStructure( G );
		X.refine();
		X1 = X.skeletalSubstructure( 2, W1 );
		X2 = X.skeletalSubstructure( 2, U );
		assert( X1.isHomogeneous() );
		assert( X2.isHomogeneous() );
	}
	std::cout << G << std::endl;
	// 8.
	return Head( G, alpha, X, X1, X2 );
}

rtype Head( ColouredBipartiteGraph G, double alpha, RelationalStructure X, RelationalStructure X1, RelationalStructure X2 ) {
	auto witness = X2.witnessOfImprimitivity();
	if( witness.first == 0 ) {
		if( X2.isUPCC() )
			return UPCCCase( G, alpha, X, X1, X2 );
		else
			return BlockDesignCase( G, alpha, X, X1, X2 );
	} else
		return ImprimitiveCase( witness.second, G, alpha, X, X1, X2 );
}

rtype UPCCCase( ColouredBipartiteGraph G, double alpha, RelationalStructure X, RelationalStructure X1, RelationalStructure X2 ) {
	std::cout << "UPCCCase(" << alpha << ")" << std::endl;
}

rtype JohnsonCase( JohnsonScheme J, ColouredBipartiteGraph G ) {
	range Gamma = J.parent_set();
	int m = Gamma.size();
	int n1 = G.vertices(ColouredBipartiteGraph::LEFT).size();
	int n2 = G.vertices(ColouredBipartiteGraph::RIGHT).size();
	int n = J.size();
	int l = std::max( log2( n1 ) * log2( n1 ), log2( n2 ) * log2( n2 ) * log2( n2 ) / log2( log2( n2 ) ) );
	auto CM = J.completeMapping();
	UnionFind uf( n );
	std::map<std::vector<int>,int> E;
	size_t i = 0;
	size_t c = 0;
	for( const auto& U : all_ordered_tuples( n, l ) ) {
		if( uf.find( i ) == i ) {
			E[U] = c;
			ColouredBipartiteGraph GU = G.substructure( G.vertices( ColouredBipartiteGraph::LEFT ), CM[ U ] );
			size_t j = 0;
			for( const auto& V : all_ordered_tuples( n, l ) ) { // TO DO: remove lower triangle
				if( j > i and uf.find( j ) == j ) { // isomorphism between i and j has not yet been checked
					ColouredBipartiteGraph GV = G.substructure( G.vertices( ColouredBipartiteGraph::LEFT ), CM[ V ] );
					if( GU.isIsomorphic( GV ) ) {
						uf.cup( i, j );
						E[ V ] = c;
					}
				}
				j++;
			}
			c++;
		}
		i++;
	}

	auto classes = uf.partitioning();
	if( classes.size() > 1 ) { // case A
		Hypergraph K( (std::deque<int>) Gamma, E );
		auto T = K.twins();
		int t = largest( T );
		if( T[t].size() * 4 > 3 * m ) { // case A1
			// case 1
		} else {
			RelationalStructure S = K.relationalStructure( T );
			// case 2
		}
	} else { // case B
		
	}

}

rtype BlockJohnsonCase() {

}

rtype BlockDesignCase( ColouredBipartiteGraph G, double alpha, RelationalStructure X, RelationalStructure X1, RelationalStructure X2 ) {
	std::cout << "BlockDesignCase(" << alpha << ")" << std::endl;
	Hypergraph H = G.neighborhoodHypergraph();
	int d = H.uniformityDegree();
	const auto& V1 = G.vertices( ColouredBipartiteGraph::LEFT );
	const auto& V2 = G.vertices( ColouredBipartiteGraph::RIGHT );
	int n1 = V1.size();
	int n2 = V2.size();
	// case 1
	if( d > 0 ) {
		// Johnson scheme
		BlockJohnsonCase();
		return rtype;
	}
	auto T = H.twins();
	int k = -1;
	for( int i : range( 0, T.size() ) ) {
		if( T[i].size() >= n2/2 ) {
			k = i;
			break;
		}
	}
	// case 2
	if( k != -1 ) {
		int r = reducePart2byColour( G, .5, T[k] );
		std::deque<int> C;
		if( r == 0 ) {
			C = std::move( T[k] ); // be warned
		else
			std::set_difference( V2.begin(), V2.end(), T[k].begin(), T[k].end(), std::back_inserter( C ) );
		ColouredBipartiteGraph Y = G.substructure( V1, C );
		return Head( Y, alpha, X, X1, X2 ); // ahhhhhhhhhhhhh
	}
	// case 3
	if( d <= 2.333333333333333333333333333333333333333 * log2( double(n1) ) ) {
		DesignLemma( magic );
	}

}

rtype ImprimitiveCase( UnionFind witness, ColouredBipartiteGraph G, double alpha, RelationalStructure X, RelationalStructure X1, RelationalStructure X2 ) {
	std::cout << "ImprimitiveCase(" << alpha << ")" << std::endl;
	const auto& Omega = X.domain();
	const auto& V1 = G.vertices( ColouredBipartiteGraph::LEFT );
	const auto& V2 = G.vertices( ColouredBipartiteGraph::RIGHT );
	int n = Omega.size();
	int n1 = V1.size();
	int n2 = V2.size();
	auto B = witness.partitioning<std::deque<std:deque<int>>();
	for( const auto& b : B )
		for( int& x : b )
			x = V2[x];
	int m = B.size();
	// 0.
	std::vector<int> d( X.relations().size(), 0 );
	std::deque<int> J;
	std::unordered_map<int> Jinv;
	for( int y : range( 0, n ) ) {
		if( X.c_edgeColour( y, y ) == v2_col ) {
			for( int x : range( 0, n ) )
				d[ X.c_edgeColour( x, y ) ] += ( X.c_edgeColour( x, x ) == v1_col );
			break;
		}
	}
	for( int i : range( 0, d.size() ) ) {
		if( d[i] ) {
			Jinv[i] = J.size();
			J.push_back( i );
		}
	}
	// 1.
	int j = -1;
	for( int i : J ) {
		if( d[j] <= n2 / 2 ) {
			j = i;
			break;
		}
	}
	if( j == -1 ) {
		rtype result;
		for( int x : V2 ) {
			std::deque<std::deque<std::deque<int>>> E( J.size(), std::deque<int>(1) );
			for( int w : V1 )
				Jinv[ X.edgeColour( x, w ) ][0].push_back( w );
			result.join( foundPartition( ColouredPartition( E ) ) );
		}
		return result;
	}
	// 2.
	bool any_found = false;
	rtype result;
	for( int i : range( 0, m ) ) {
		ColouredBipartiteGraph Z( V1, B[i], X.relation(j) );
		if( Z.symmetryDefect() >= .5 ) {
			any_found = true;
			result.join( Head( Z, alpha, X, X1, X2 ) ); // warning, recompute alpha,...,X2
		}
	}
	if( not any_found ) {
		int h = J.back();
		if( h == j )
			h = *(J.end()-1);
		std::deque<int> M( m );
		std::iota( M.begin(), M.end(), 0 );
		ColouredBipartiteGraph::parameter E;
		for( int x : V1 ) {
			for( int p : range( 0, m ) ) {
				for( int y : B[p] ) {
					if( X.edgeColour( x, y ) == h ) {
						E[x].push_back( p );
						break;
					}
				}
			}
		}
		ColouredBipartiteGraph Y( V1, M, E );
		result = Head( Y, alpha, X, X1, X2 ); // warning, recomp....
	}
	return result;
}


int main() {
	int n = 12;
	int n2 = n*n;
	std::vector<int> r( n2*n2, 0 );
	for( int i = 0; i < n2; i++ )
		for( int j = 0; j < n2; j++ )
			if( i != j ) {
				if( i % n == j % n or i / n == j / n )
					r[i+n2*j] = 1;
				else
					r[i+n2*j] = 2;
			}
	std::deque<int> C( n2 );
	std::iota( C.begin(), C.end(), -n2 );
	RelationalStructure X( std::move( C ), std::move( r ), 2 );
	//std::cout << X << std::endl;
	std::cout << "---------------------------------" << std::endl;
	X.refine();
	//std::cout << X << std::endl;
	std::cout << "---------------------------------" << std::endl;
	UPCCtoBipartite( X, 0.6666666666666666666666666666 );
}