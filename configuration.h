#pragma once
#include <vector>
#include <set>
#include <map>
#include "ext.h"
#include "matrix.h"

class RelationalStructure;
class ColoredPartition;
class Graph;
class BipartiteGraph;

class RelationalStructure {
	int _n;
	int _dimension;
	std::vector<std::set<std::vector<int>>> _relations;

	size_t c( const std::vector<int>& t ) const;
	void refine( bool print );
public:
	size_t domainSize() const;
	size_t arity() const;
	bool isBinary() const;
	bool isUniprimitive() const;
	bool isHomogeneous() const;
	bool isPrimitive() const;
	void individualize( std::vector<int> );
	RelationalStructure skeletalSubstructure( int t, std::vector<int> Delta ) const;
	void WeisfellerLeman( bool print = false );
	const std::vector<std::set<std::vector<int>>>& relations() const;
	RelationalStructure( std::vector<std::set<std::vector<int>>> relations, int n );
	RelationalStructure( const Graph& G, int k );
};

class ColoredPartition {
	int _n;
	std::vector<std::vector<std::set<int>>> _partition_to_elements;
	std::vector<std::pair<int,int>> _element_to_partitions;
public:
	void individualize( int i );
	ColoredPartition( std::vector<std::vector<std::set<int>>> partitions );
};

class Graph {
	matrix<bool> adjacency;
public:
	size_t vertices() const;
	std::vector<bool>::reference edge( int u, int v );
	std::vector<bool>::const_reference edge( int u, int v ) const;
	const matrix<bool>& adjacencyMatrix() const;
	Graph( int n );
};

class BipartiteGraph {
	std::vector<bool> _vertex_part;
	std::multimap<int,int> _edges;
	ColoredPartition _colors;
public:
	size_t domainSize() const;
	const ColoredPartition& coloring() const;
	bool vertex( int i ) const;
};



class JohnsonScheme {

};

Either<ColoredPartition,JohnsonScheme> bipartiteSplitOrJohnson( const BipartiteGraph& X, double alpha );
Either<ColoredPartition,RelationalStructure> splitOrUPCC( const RelationalStructure& X, double alpha );