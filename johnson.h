#pragma once
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <utility>
#include <future>
#include "group.h"
#include "action.h"
#include "ext.h"


std::set<std::set<int>> CameronThread( std::vector<std::array<int,2>>::const_iterator begin, std::vector<std::array<int,2>>::const_iterator end, const std::unordered_map<int,std::vector<int>>& Delta ) {
	int counter = 0;
	std::unordered_set<int> B;
	std::set<int> C;
	std::set<std::set<int>> D_prime;
	for(; begin != end; ++begin ) {
		if( (++counter) % 10 == 0 )
			std::cerr << counter << std::endl; 
		int x = (*begin)[0];
		int y = (*begin)[1];
		B = {};
		C = {};
		for( int q : Delta.at(y) )
			B.insert( q );
		for( int q : Delta.at(x) )
			B.erase( q );
		// C.insert( phi.domain().begin(), phi.domain().end() );
		for( int z : B )
			for( int q : Delta.at(z) )
				C.insert( q );
		D_prime.emplace( std::move( C ) );
	}
	return D_prime;
}

template<typename T>
Action<std::set<T>> CameronReduction( Action<T> phi ) {
	/*std::cerr << "CameronReduction\n";
	auto psi = phi.template arrayAction<2>();
	std::cerr << "Action\n"; 
	auto orbitals = psi.orbits();
	std::cerr << "Orbitals\n"; */
	int n = phi.domain().size();
	auto orbitals = phi.hack__orbitals();
	std::sort( orbitals.begin(), orbitals.end(), size_compare<std::vector<std::array<T,2>>> );
	const auto& Gamma = orbitals[1];
	const auto& Delta_prime = orbitals.back();
	std::unordered_map<T,std::vector<T>> Delta;
	for( const auto& delta : Delta_prime )
		Delta[delta[0]].push_back( delta[1] );
	std::set<T> B;
	std::set<T> C;
	std::set<std::set<T>> D_prime;
	std::cerr << "|Gamma|=" << Gamma.size() << std::endl; 
	std::cerr << "|Delta|=" << Delta_prime.size() << std::endl; 
	int counter = 0;
	const int pt = 16;
	int q = Gamma.size() / pt;
	auto a = Gamma.begin();
	auto b = Gamma.begin();
	std::future<std::set<std::set<T>>> fut[pt-1];
	for( int i = 0; i < pt-1; i++ ) {
		b += q;
		fut[i] = std::async(std::launch::async,CameronThread, a, b, Delta );
		a = b;
	}
	D_prime = CameronThread( a, b, Delta );
	for( int i = 0; i < pt-1; i++ ) {
		auto x = fut[i].get();
		D_prime.insert( x.begin(), x.end() );
	}

	/*for( const auto& p : Gamma ) {


		if( (++counter) % 10 == 0 )
			std::cerr << counter << std::endl; 
		const T& x = p[0];
		const T& y = p[1];
		B = {};
		C = {};
		std::cout << "/" << std::flush;
		for( const T& q : Delta[y] )
			B.insert( q );
		std::cout << "|" << std::flush;
		for( const T& q : Delta[x] )
			B.erase( q );
		std::cout << "\\" << std::flush;
		C.insert( phi.domain().begin(), phi.domain().end() );
		std::cout << "|" << std::flush;
		for( const auto& z : B )
			for( const T& q : Delta[z] )
				C.erase( q );
		// D_prime.emplace( std::move( C ) );
	}*/
	std::cout << "Done" << std::endl;
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