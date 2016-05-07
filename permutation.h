#pragma once

#include <iostream>
#include <vector>
#include <initializer_list>
#include <set>
#include <exception>
#include <deque>

class all_permutations;

class Permutation {
	friend class all_permutations;

	std::vector<int> _map;
	mutable int _order;
public:
	int degree() const;
	int order() const;
	bool isIdentity() const;
	const std::vector<int>& getArrayNotation() const;
	std::vector<std::vector<int>> getCycleNotation() const;

	bool operator<( const Permutation& ) const;
	bool operator==( const Permutation& ) const;
	bool operator!=( const Permutation& ) const;

	Permutation& operator*=( const Permutation& );
	Permutation operator*( const Permutation& ) const;
	Permutation& operator^=( int );
	Permutation operator^( int ) const;
	Permutation inverse() const;
	Permutation project( const std::vector<int>& Delta ) const;
	int operator()( int ) const;
	Permutation( int n );
	Permutation( std::vector<int>&& );
	Permutation( std::initializer_list<int> );
};

class all_permutations {
	int _n;
public:
	class iterator {
		int _n;
		Permutation _p;
	public:
		typedef iterator self_type;
		typedef Permutation value_type;
		typedef Permutation& reference;
		typedef Permutation* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef size_t difference_type;
		iterator( int n );
		iterator( const self_type& other );
		self_type& operator++();
		self_type operator++(int);
		reference operator*();
		pointer operator->();
		bool operator==(const self_type& rhs);
		bool operator!=(const self_type& rhs);
	};
	iterator begin();
	iterator end();
	all_permutations( int n );
};

std::ostream& operator<<( std::ostream& os, const Permutation& cycles );