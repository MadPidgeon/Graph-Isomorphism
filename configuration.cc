#include "configuration.h"
#include <vector>
#include <set>
#include <map>
#include "ext.h"

size_t RelationalStructure::c( const std::vector<int>& t ) const {
	for( size_t i = 0; i < _relations.size(); i++ )
		if( _relations[i].count( t ) == 1 )
			return i;
	throw;
}

size_t RelationalStructure::domainSize() const {
	return _n;
}

size_t RelationalStructure::arity() const {
	return _dimension;
}

void RelationalStructure::WeisfellerLeman() {
	size_t relation_count;
	do {
		relation_count = _relations.size();
		refine();
		std::cerr << "+";
	} while( relation_count != _relations.size() );
}

void RelationalStructure::refine() {
	int n = _n;
	int r = _relations.size();
	int k = _dimension;
	int n_pow_k = pow( n, k );
	std::vector<std::vector<int>> index_to_color;
	std::map<std::vector<int>,int> color_to_index;
	std::vector<int> element;
	std::vector<int> new_color_mapping( n_pow_k, 0 );
	int rel_index = 0;
	// refinement
	for( auto& relation : all_tuples( n, k ) ) {
		std::vector<int> color( 1, c( relation ) );
		std::vector<int> x = relation;
		for( auto& i : all_tuples( r, k ) ) {
			int number = 0;
			for( int y = 0; y < n; y++ ) {
				int add = 1;
				for( int j = 0; j < k; j++ ) {
					x[j] = y;
					int v = c( x );
					x[j] = relation[j];
					if( v != i[j] ) {
						add = 0;
						break;
					}
				}
				number += add;
			}
			if( number )
				color.emplace_back( number );
		}
		std::sort( color.begin()+1, color.end() );
		auto f = color_to_index.find( color );
		int new_color;
		if( f == color_to_index.end() ) {
			new_color = index_to_color.size();
			color_to_index[color] = new_color;
			index_to_color.emplace_back( std::move( color ) );
		} else
			new_color = f->second;
		new_color_mapping[rel_index] = new_color;
		rel_index++;
	}
	// std::cerr << index_to_color << std::endl;
	// normalizing indices
	auto index_ranking = rank( index_to_color );
	std::vector<std::set<std::vector<int>>> new_relations( index_to_color.size() );
	rel_index = 0;
	for( auto& relation : all_tuples( n, k ) ) {
		new_relations[index_ranking[new_color_mapping[rel_index]]].insert( relation );
		rel_index++;
	}
	_relations = std::move( new_relations );
}

RelationalStructure::RelationalStructure( std::vector<std::set<std::vector<int>>> relations, int n ) {
	_relations = std::move( relations );
	_n = n;
	_dimension = _relations.begin()->begin()->size();
}

RelationalStructure::RelationalStructure( const Graph& G, int k ) {
	bool found, valid;
	size_t isomorphism_class;
	_n = G.vertices();
	_dimension = k;
	for( const auto& s1 : all_tuples( _n, _dimension ) ) {
		found = false;
		for( isomorphism_class = 0; isomorphism_class < _relations.size(); isomorphism_class++ ) {
			const auto& s2 = *_relations[ isomorphism_class ].begin();
			valid = true;
			for( const auto& p : all_ordered_tuples( _dimension, 2 ) ) {
				if( not ( 
						(s1[p[0]] == s1[p[1]]) == (s2[p[0]] == s2[p[1]]) and
						(G.edge(s1[p[0]],s1[p[1]]) == G.edge(s2[p[0]],s2[p[1]]))
					) ) {
					valid = false;
					break;
				}
			}
			if( valid ) {
				found = true;
				break;
			}
		}
		if( found )
			_relations[isomorphism_class].insert( s1 );
		else
			_relations.emplace_back( std::move( std::set<std::vector<int>>( { s1 } ) ) );
	}
}

const std::vector<std::set<std::vector<int>>>& RelationalStructure::relations() const {
	return _relations;
}

void RelationalStructure::individualize( std::vector<int> S ) {
	for( int s : S ) {
		std::vector<int> x( _dimension, s );
		int color = c( x );
		if( _relations[color].size() != 1 ) {
			_relations[color].erase( x );
			_relations.emplace_back( std::move( std::set<std::vector<int>>({ x }) ) );
		}
	}
}

RelationalStructure RelationalStructure::skeletalSubstructure( int t, std::vector<int> Delta ) const {
	std::map<int,int> D;
	for( size_t i = 0; i < Delta.size(); i++ )
		D[Delta[i]] = i;
	std::vector<std::set<std::vector<int>>> new_relations;
	std::set<std::vector<int>> new_relation;
	std::vector<int> new_tuple( t );
	for( const auto& rel : _relations ) {
		for( const auto& tup : rel ) {
			int i = 0;
			bool valid = true;
			for(; i < t; i++ ) {
				if( D.count( tup[i] ) == 0 ) {
					valid = false;
					break;
				} else
					new_tuple[i] = D[tup[i]]; 
			}
			if( valid ) {
				for(; i < _dimension; i++ ) {
					if( tup[i] != tup[t] ) {
						valid = false;
						break;
					}
				}
				if( valid )
					new_relation.insert( new_tuple );
			}
		}
		if( new_relation.size() > 0 )
			new_relations.emplace_back( std::move( new_relation ) );
	}
	return RelationalStructure( std::move( new_relations ), Delta.size() );
}

void ColoredPartition::individualize( int i ) {
	auto& p = _element_to_partitions[i];
	auto& old_part = _partition_to_elements[p.first][p.second];
	if( old_part.size() > 1 ) {
		old_part.erase( i );
		p.second = _partition_to_elements[p.first].size();
		_partition_to_elements[p.first].emplace_back( std::set<int>( { i } ) );
	}
}

ColoredPartition::ColoredPartition( std::vector<std::vector<std::set<int>>> partitions ) {
	_partition_to_elements = std::move( partitions );
	int maximum = -1;
	_n = 0;
	for( const auto& color : _partition_to_elements )
		for( const auto& part : color )
			_n += part.size();
	_element_to_partitions.resize( _n );
	for( size_t c = 0; c < _partition_to_elements.size(); c++ )
		for( size_t p = 0; p < _partition_to_elements[c].size(); p++ )
			for( int x : _partition_to_elements[c][p] )
				_element_to_partitions[x] = std::make_pair( c, p );
}

size_t BipartiteGraph::domainSize() const {
	return _vertex_part.size();
}

bool BipartiteGraph::vertex( int i ) const {
	return _vertex_part.at( i );
}

const ColoredPartition& BipartiteGraph::coloring() const {
	return _colors;
}

bool isVertex( const std::vector<int>& v ) {
	int y = v.back();
	for( const int x : v )
		if( x != y )
			return false;
	return true;
}

size_t Graph::vertices() const {
	return adjacency.width();
}

std::vector<bool>::reference Graph::edge( int u, int v ) {
	return adjacency.at( u, v );
}

std::vector<bool>::const_reference Graph::edge( int u, int v ) const {
	return adjacency.at( u, v );
}

const matrix<bool>& Graph::adjacencyMatrix() const {
	return adjacency;
}

Graph::Graph( int n ) : adjacency( n, n ) {
}

std::vector<std::set<int>> connectedComponents( int n, const std::set<std::vector<int>>& E ) {
	std::vector<std::set<int>> connected_components;
	std::vector<bool> d( n*n, false );
	for( int i = 0; i < n; i++ )
		d[(n+1)*i] = true;
	for( const auto& edge : E )
		d[edge[0]+n*edge[1]] = true;
	for( int i = 0; i < n; i++ )
		for( int j = 0; j < n; j++ )
			for( int k = 0; k < n; k++ )
				d[j+k*n] = d[j+k*n] or ( d[j+i*n] and d[i+k*n] );
	for( int i = 0; i < n; i++ )
		if( d[(n+1)*i] ) {
			std::set<int> component;
			for( int j = i; j < n; j++ )
				if( d[j+n*i] and d[i+n*j] ) {
					component.insert(j);
					d[(n+1)*j] = false;
				}
			connected_components.emplace_back( std::move( component ) );
		}
	return connected_components;
}

Either<Either<ColoredPartition,RelationalStructure>,std::vector<int>> splitOrUPCC_subroutine( const RelationalStructure& XS, double alpha ) {
	size_t n = XS.domainSize();
	size_t m = n * alpha;
	std::vector<int> CS;
	// construct a coloring from XS
	int violating_relation = -1;
	int violating_coloring;
	std::vector<std::vector<std::set<int>>> coloring;
	int i = 0;
	for( const auto& rel : XS.relations() ) {
		if( isVertex( *rel.cbegin() ) ) {
			std::set<int> block;
			for( const auto& tup : rel )
				block.insert( tup[0] );
			if( block.size() > m ) {
				violating_relation = i;
				violating_coloring = coloring.size();
			}
			std::vector<std::set<int>> color;
			color.emplace_back( std::move( block ) );
			coloring.emplace_back( std::move( color ) );
		}
		i++;
	}
	// check if no vertex-color in XS has measure > alpha
	if( violating_relation == -1 )
		return Either<ColoredPartition,RelationalStructure>( ColoredPartition( coloring ) );
	else
		for( const auto& tup : XS.relations()[violating_relation] )
			CS.push_back( tup[0] );
	// we have a vertex-color class CS of measure > alpha
	auto Xstar = XS.skeletalSubstructure( 2, CS );
	int nstar = Xstar.domainSize();
	for( const auto& constituent : XS.relations() ) {
		if( isVertex( *constituent.cbegin() ) )
			continue;
		auto cc = connectedComponents( nstar, constituent );
		if( cc.size() != 1 ) { // if Xstar is imprimitive
			std::swap( cc, coloring[violating_coloring] );
			return Either<ColoredPartition,RelationalStructure>( ColoredPartition( coloring ) );
		}
	}
	// Xstar is primitive
	if( Xstar.relations().size() >= 3 ) // Xstar is uniprimitive
		return Either<ColoredPartition,RelationalStructure>( Xstar );
	else
		return CS;
}

Either<ColoredPartition,RelationalStructure> splitOrUPCC( const RelationalStructure& X, double alpha ) {
	int k = X.arity();
	int n = X.domainSize();
	RelationalStructure X0 = X;
	X0.WeisfellerLeman();
	auto r = splitOrUPCC_subroutine( X0, alpha );
	std::vector<int> C;
	if( r.isFirst() )
		return r.getFirst();
	else
		C = r.getSecond();
	for( int i = 1; i < k; i++ ) {
		for( auto& S : all_ordered_tuples( n, i ) ) {
			auto XS = X;
			XS.individualize( S );
			auto q = splitOrUPCC_subroutine( XS, alpha );
			if( q.isFirst() )
				return q.getFirst();
		}
	}
	throw;
}

Either<ColoredPartition,JohnsonScheme> bipartiteSplitOrJohnson( const BipartiteGraph& X, double alpha, int C0 ) {
	int n = X.domainSize();
	int n1 = 0, n2 = 0;
	for( int i = 0; i < n; i++ )
		if( X.vertex( i ) == 0 )
			n1++;
	n2 = n - n1;
	// 1.
	if( n1 <= C0 ) {
		ColoredPartition cp( X.coloring() );
		int c = 0;
		int i = 0;
		int b = (1-alpha) * n1;
		while( c < b and i < n ) {
			if( X.vertex( i ) == 0 ) {
				cp.individualize( i );
				c++;
			}
			i++;
		}
		return cp;
	}
	// 2.
	if( n2 <= n1 ) {
		#warning "Split or Johnson requires quasipolynomial funtion"
		

	}
}