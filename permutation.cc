#include "permutation.h"
#include <vector>
#include <initializer_list>
#include <exception>
#include <stdexcept>

int Permutation::degree() const {
	return _map.size();
}

bool Permutation::isIdentity() const {
	for( int i = 0; i < degree(); i++ )
		if( (*this)(i) != i )
			return false;
	return true;
}

int Permutation::order() const {
	if( _order != -1 )
		return _order;
	Permutation p = (*this);
	_order = 1;
	while( !p.isIdentity() ) {
		p *= (*this);
		_order++;
	}
	return _order;
}

bool Permutation::operator<( const Permutation& other ) const {
	if( degree() != other.degree() )
		throw std::range_error( "Permutations not compatible" );
	for( int i = 0; i < degree(); i++ )
		if( (*this)(i) < other(i) )
			return true;
		else if( (*this)(i) > other(i) )
			return false;
	return false;
}

bool Permutation::operator==( const Permutation& other ) const {
	if( degree() != other.degree() )
		throw std::range_error( "Permutations not compatible" );
	return _map == other._map;
}

bool Permutation::operator!=( const Permutation& other ) const {
	return !( *this == other);
}

int Permutation::operator()( int k ) const {
	return _map[k];
}

std::vector<std::vector<int>> Permutation::getCycleNotation() const {
	std::vector<std::vector<int>> cycles;
	std::set<int> done;
	for( int i = 0; i < degree(); i++ ) {
		if( done.count( i ) == 0 ) {
			int j = i;
			std::vector<int> cycle;
			do {
				done.insert( j );
				cycle.push_back( j );
				j = (*this)(j);
			} while( i != j );
			if( cycle.size() != 1 )
				cycles.push_back( std::move( cycle ) );
		}
	}
	return cycles;
}

Permutation Permutation::operator*( const Permutation& sigma ) const {
	if( degree() != sigma.degree() )
		throw std::range_error( "Permutations not compatible" );
	std::vector<int> v( degree() );
	for( int i = 0; i < degree(); i++ )
		v[i] = (*this)(sigma(i));
	return Permutation( std::move(v) );
}

Permutation& Permutation::operator*=( const Permutation& sigma ) {
	Permutation p = (*this) * sigma;
	_map = move( p._map );
	_order = -1;
	return *this;
}

Permutation Permutation::operator^( int k ) const {
	Permutation p = (*this);
	std::vector<int> v( degree() );
	for( int i = 0; i < degree(); i++ )
		v[i] = i;
	Permutation t( std::move( v ) );
	if( k < 0 ) {
		p = p.inverse();
		k = -k;
	}
	for( int i = 0; i < k; i++ )
		t *= p;
	return t;
}

Permutation& Permutation::operator^=( int k ) {
	Permutation p = (*this) ^ k;
	_map = move( p._map );
	_order = -1;
	return *this;
}

Permutation Permutation::inverse() const {
	std::vector<int> v( degree() );
	for( int i = 0; i < degree(); i++ )
		v[ (*this)(i) ] = i;
	return Permutation( std::move( v ) );
}

Permutation::Permutation( std::vector<int>&& m ) {
	_order = -1;
	_map = m;
}

Permutation::Permutation( std::initializer_list<int> l ) {
	_order = -1;
	_map = std::vector<int>( l );
}

std::ostream& operator<<( std::ostream& os, const Permutation& sigma ) {
	auto cycles = sigma.getCycleNotation();
	if( cycles.size() == 0 )
		return os << "()";
	else for( auto& cycle : cycles ) {
		os << "( ";
		for( int x : cycle )
			os << x << " ";
		os << ")";
	}
	return os;
}