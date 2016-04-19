#pragma once
#include <map>
#include "configuration.h"
#include "ext.h"
#include "permutation.h"

typedef std::map<std::vector<int>,int> inv_gamma_t;
typedef std::deque<std::vector<int>> gamma_t;
typedef int rtype2;

template<typename T>
Either<rtype2,Empty> align( const T& Xx, const T& Xy ) {
	throw;
	return Empty();
}


Permutation pullback( Permutation sigma_bar, const inv_gamma_t& inv_gamma, const gamma_t& gamma );

/*
template<>
Either<rtype2,Empty> Align<ColoredPartition>( const ColoredPartition& Xx, const ColoredPartition& Xy, const inv_gamma_t& inv_gamma, const gamma_t& gamma ) {
	// 1.
	// TO DO: check alternating type
	Either<Permutation,Empty> r = Xx.getIsomorphism( Xy );
	if( r.isSecond() )
		return Empty();
	Permutation sigma_bar = r.getFirst(); 
	// 2.
	std::cout << pullback( sigma_bar, inv_gamma, gamma );
	return 0;
}*/