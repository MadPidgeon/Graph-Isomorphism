#pragma once
#include <vector>
#include <deque>
#include <unordered_map>
#include "ext.h"
#include "unionfind.h"

typedef std::deque<std::deque<int>> regular_mapping_t;
typedef std::unordered_map<int,int> inverse_mapping_t;


class ColouredSet {
	regular_mapping_t mapping;
	inverse_mapping_t inverse_mapping;
public:
	int getColour( int x ) const;
	const std::deque<int>& getColourClass( int i ) const;
	template<typename T> void individualise( T S );
	const regular_mapping_t& getMapping() const;
	ColouredSet substructure( std::deque<int> elms ) const;
	range colours() const;
	ColouredSet( std::deque<std::deque<int>> );
	ColouredSet() = default;
	ColouredSet(ColouredSet&&);
	ColouredSet& operator=(ColouredSet&&) = default;
	ColouredSet(const ColouredSet&) = default;
};

class ColouredPartition {
	std::deque<std::deque<std::deque<int>>> partition_mapping;
	std::unordered_map<int,std::pair<int,int>> inverse_partition_mapping;
public:
	bool is_equipartition() const;
	void equipartition();
	const std::deque<regular_mapping_t>& getMapping() const;
	ColouredPartition( std::initializer_list<std::deque<std::deque<int>>> L );
	ColouredPartition( ColouredSet );
	ColouredPartition( ColouredPartition&& ) = default;
	ColouredPartition( const ColouredPartition& ) = default;
	explicit operator ColouredSet() const;
};

class Hypergraph {
	std::deque<int> Omega;
	std::map<std::vector<int>,int> E; // edge colouring
public:
	int uniformityDegree() const;
	Hypergraph( std::deque<int> vertices, std::map<std::vector<int>,int> edges );
	Hypergraph( std::map<std::vector<int>,int> edges );
private:
	void gatherVertices();
};

class ColouredBipartiteGraph {
public:
	enum side { LEFT = 0, RIGHT = 1 };
	typedef std::unordered_map<int,std::deque<int>> parameter; // only one direction ness
private:
	std::deque<int> Omega[2];
	std::unordered_map<int,std::pair<side,int>> agemO;
	std::deque<std::deque<int>> nbh[2];
	ColouredSet cs;
	int n1, n2;
public:
	std::deque<std::deque<int>> twins( side s ) const;
	double symmetryDefect( side s ) const;
	int colour( int x ) const;
	const std::deque<int>& vertices( side s ) const;
	side getSide( int x ) const;
	std::deque<int> neighborhood( int v ) const;
	bool hasEdge( int v, int w ) const;
	Hypergraph neighborhoodHypergraph() const;
	ColouredBipartiteGraph substructure( std::deque<int> W1, std::deque<int> W2 ) const;
	ColouredBipartiteGraph( std::deque<int> V1, std::deque<int> V2, parameter E, ColouredSet c );
	ColouredBipartiteGraph( std::deque<int> V1, std::deque<int> V2, parameter E );
	ColouredBipartiteGraph( ColouredBipartiteGraph&& ) = default;
	ColouredBipartiteGraph( const ColouredBipartiteGraph& ) = default;
	ColouredBipartiteGraph& operator=( ColouredBipartiteGraph&& );
	ColouredBipartiteGraph& operator=( const ColouredBipartiteGraph& ) = default;
	explicit operator ColouredSet() const;
private:
	bool c_hasEdge( int x, int y ) const;
	bool c_areTwins( side s, int x, int y ) const;
	const std::deque<int>& c_neighborhood( side s, int x ) const;
	int decode( side s, int x ) const;
	std::pair<side,int> encode( int x ) const;
};

class RelationalStructure {
	std::deque<int> Omega;
	std::unordered_map<int,int> agemO;
	size_t k; // arity
	std::vector<int> r; // k-dim relation matrix
	// int n;
	int colour_relations_end;
	int all_relations_end;
	struct Relation : public std::deque<std::vector<int>> {
		std::deque<int> vertices() const; 
	};
public:
	size_t arity() const;
	const std::deque<int>& domain() const;
	range vertexColours() const;
	range relations() const;
	range edgeColours() const;
	Relation relation( size_t i ) const;
	bool isHomogeneous() const;
	bool isPrimitive() const;
	std::pair<int,UnionFind> witnessOfImprimitivity() const;
	bool isClique() const;
	bool isUPCC() const;
	template<typename T> void individualise( T S );
	int vertexColour( int x ) const;
	int edgeColour( int a, int b ) const;
	ColouredBipartiteGraph inducedBipartiteGraph( std::deque<int>&& V1, std::deque<int>&& V2, int r ) const;
	RelationalStructure( const ColouredBipartiteGraph& G );
	explicit operator ColouredSet() const;
	int refine();
	RelationalStructure skeleton( size_t t ) const;
	RelationalStructure skeletalSubstructure( size_t t, std::deque<int> C ) const;
	std::deque<std::deque<int>> twins( int i ) const;
	int getNonAlphaPartition( double alpha );
	RelationalStructure( std::deque<int>&& C, std::vector<int>&& r, int k );
	int c_edgeColour( int x, int y ) const;
private:
	void normalise();
	int encode( int x ) const;
	int decode( int y ) const;
	int& c_vertexColour( int x );
	bool WeisfeilerLehman();
};

struct JohnsonScheme : all_tuples {
	std::deque<int> mapping; // the n-th subset of Gamma as measured by all_ordered_tuples mapping to V2
	std::map<int,int> inverse_mapping; // the elements of V2 mapping to the n-th subset of Gamma
	JohnsonScheme( int m, int t );
	std::map<std::vector<int>,int> completeMapping() const;
};

std::ostream& operator<<( std::ostream& os, const ColouredBipartiteGraph& G );
std::ostream& operator<<( std::ostream& os, const RelationalStructure& X );


template<typename T>
void ColouredSet::individualise( T S ) {
	for( int x : S ) {
		int c = getColour( x );
		if( mapping.size() > 1 ) {
			mapping[c].erase( std::find( mapping[c].begin(), mapping[c].end(), x ) );
			inverse_mapping[x] = mapping.size();
			mapping.emplace_back( std::deque<int>({x}) );
		}
	}
}

template<typename T> void RelationalStructure::individualise( T S ) {
	//size_t add = S.size();
	// double individualisation errors
	//int start = vertexColours().size();
	//for( int& x : r )
	//	if( x >= start )
	//		x += add;
	//add = 0;
	// std::cout << all_relations_end << std::endl;
	int add = all_relations_end;
	for( int x : S ) {
		std::vector<int> x_vec( arity(), encode( x ) );
		size_t off = polynomial_evaluation( x_vec, domain().size() );
		r[ off ] += add++;
	}
	normalise();
}