#pragma once
#include <vector>
#include <set>
#include <map>
#include "group.h"
#include "action.h"
#include "ext.h"

template<typename T>
Action<std::set<T>> CameronReduction( Action<T> phi ) {
	auto psi = phi.tupleAction(2);
	auto orbitals = psi.orbits();
	std::sort( orbitals.begin(), orbitals.end(), size_compare<std::vector<std::vector<int>>> );
	const auto& Gamma = orbitals[1];
	const auto& Delta = orbitals.back();
	std::set<T> B;
	std::set<T> C;
	std::set<T> D_prime;
	/*for( const auto& p : Gamma ) {
		const T& x = p[0];
		const T& y = p[1];
		B = {};
		C = {};
		for( const auto& q : Delta )
			if( q[0] == y )
				B.insert( q[1] );
		for( const auto& q : Delta )
			if( q[0] == x )
				B.erase( q[1] );
		C.insert( phi.domain().begin(), phi.domain().end() );
		for( const auto& z : B )
			for( const auto& q : Delta )
				if( q[0] == z )
					C.erase( q[1] );
		D_prime.emplace( std::move( C ) );
	}*/
	std::vector<T> D( D_prime.begin(), D_prime.end() );
	Action<T> chi_prime( phi.group(), D, phi.function() );
	Action<std::set<T>> chi = chi_prime.reverseSystemOfImprimitivity();
	return chi;
}

std::set<int> JordanLiebeckSet( Action<std::set<int>> phi, int x );
std::vector<std::set<int>> JohnsonStandardBlocks( Action<std::set<int>> phi );