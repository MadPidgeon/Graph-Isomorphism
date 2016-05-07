#include <stdexcept>
#include <iostream>
#include "coset.h"
#include "group.h"
#include "permutation.h"
#include "ext.h"

Group Coset::supergroup() const {
	return _G;
}

Group Coset::subgroup() const {
	return _H;
}

bool Coset::isRightCoset() const {
	return _right;
}

const Permutation& Coset::representative() const {
	return _sigma;
}

bool Coset::operator==( const Coset& other ) const {
	if( subgroup()->hasSubgroup(other.subgroup()) && other.subgroup()->hasSubgroup( subgroup() ) && isRightCoset() == other.isRightCoset() )
		return subgroup()->contains( representative().inverse() * other.representative() );
	throw std::range_error( "Cosets are incomparable" );
}

Coset::Coset( Group G, Group H, Permutation sigma, bool right ) : _sigma( std::move( sigma ) ) {
	_G = std::move( G );
	_H = std::move( H );
	if( _H == nullptr or !_G->hasSubgroup( _H ) )
		throw std::range_error( "Can't construct coset since argument is not a subgroup" );
	_right = right;
}

std::ostream& operator<<( std::ostream& os, const Coset& c ) {
	if( c.isRightCoset() )
		return os << c.subgroup()->generators() << c.representative();
	else
		return os << c.representative() << c.subgroup()->generators();
}


bool Iso::isEmpty() const {
	return isSecond();
}

const Coset& Iso::coset() const {
	return getFirst();
}

Coset operator*( const Permutation& sigma, const Coset& tauH ) {
	// std::cout << tauH << std::endl;
	assert( not tauH.isRightCoset() );
	Permutation sigmatau = sigma * tauH.representative();
	return Coset( tauH.supergroup(), tauH.subgroup(), sigmatau, false );
}

Iso operator*( const Permutation& sigma, const Iso& tauH ) {
	if( tauH.isEmpty() )
		return tauH;
	else
		return sigma * tauH.coset();
}
