#pragma once

#include <iostream>
#include <vector>
#include <initializer_list>
#include <set>
#include <exception>

class Permutation {
	std::vector<int> _map;
	mutable int _order;
public:
	int degree() const;
	int order() const;
	bool isIdentity() const;
	std::vector<std::vector<int>> getCycleNotation() const;

	bool operator<( const Permutation& ) const;
	bool operator==( const Permutation& ) const;
	bool operator!=( const Permutation& ) const;

	Permutation& operator*=( const Permutation& );
	Permutation operator*( const Permutation& ) const;
	Permutation& operator^=( int );
	Permutation operator^( int ) const;
	Permutation inverse() const;
	int operator()( int ) const;
	Permutation( std::vector<int>&& );
	Permutation( std::initializer_list<int> );
};

std::ostream& operator<<( std::ostream& os, const Permutation& cycles );