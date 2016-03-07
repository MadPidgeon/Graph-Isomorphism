#pragma once
#include <vector>
#include <set>
#include <map>
#include <utility>
#include "group.h"
#include "action.h"
#include "ext.h"

template<typename T>
Action<std::set<T>> CameronReduction( Action<T> phi ) {
	std::cerr << "CameronReduction\n"; 
	auto psi = phi.tupleAction(2);
	std::cerr << "Action\n"; 
	auto orbitals = psi.orbits();
	std::cerr << "Orbitals\n"; 
	std::sort( orbitals.begin(), orbitals.end(), size_compare<std::vector<std::vector<T>>> );
	const auto& Gamma = orbitals[1];
	const auto& Delta_prime = orbitals.back();
	std::map<T,std::vector<T>> Delta;
	for( const auto& delta : Delta_prime )
		Delta[delta[0]].push_back( delta[1] );
	std::set<T> B;
	std::set<T> C;
	std::set<std::set<T>> D_prime;
	std::cerr << "|Gamma|=" << Gamma.size() << std::endl; 
	std::cerr << "|Delta|=" << Delta_prime.size() << std::endl; 
	for( const auto& p : Gamma ) {
		// std::cerr << "."; 
		const T& x = p[0];
		const T& y = p[1];
		B = {};
		C = {};
		for( const T& q : Delta[y] )
			B.insert( q );
		for( const T& q : Delta[x] )
			B.erase( q );
		C.insert( phi.domain().begin(), phi.domain().end() );
		for( const auto& z : B )
			for( const T& q : Delta[z] )
				C.erase( q );
		D_prime.emplace( std::move( C ) );
	}
	std::vector<std::set<T>> D( D_prime.begin(), D_prime.end() );
	Action<std::set<T>> chi_prime = phi.setwiseAction( D );
	std::cerr << chi_prime.domain() << std::endl; 
	Action<std::set<std::set<T>>> chi = chi_prime.reverseSystemOfImprimitivity();
	std::cerr << chi.domain() << std::endl; 
	D.clear();
	for( const auto& ss : chi.domain() )
		D.emplace_back( std::move( flatten( ss ) ) );
	return Action<std::set<T>>( chi_prime.group(), D, chi_prime.function() );
}

std::set<int> JordanLiebeckSet( Action<std::set<int>> phi, int x );
std::vector<std::set<int>> JohnsonStandardBlocks( Action<std::set<int>> phi );