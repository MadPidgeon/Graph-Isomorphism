#pragma once
#include <iostream>

class UnionFind {
	friend std::ostream& operator<<( std::ostream& os, const UnionFind& uf );
	int N;
	int* set;
	public:
	int find( int s ) const;
	void cup( int, int );
	void clear();
	UnionFind( int n );
	~UnionFind();
	UnionFind( const UnionFind& );
};
