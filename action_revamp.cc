#pragma once

#include <functional>
#include <set>
#include <vector>
#include <stack>
#include <exception>
#include <map>
#include <memory>
#include <unordered_map>

#include "ext.h"
#include "unionfind.h"
#include "group.h"
#include "action.h"

// ----------------------------------------------------------------

NaturalAction::domain_type NaturalAction::domain() const {
	return range( 0, group()->degree() );
}

Group NaturalAction::anonymize() const {
	return group();
}

NaturalAction::NaturalAction( Group G ) : PointAction<NaturalAction,NaturalAction::value_type,NaturalAction::domain_type>( G ) {
}

// ----------------------------------------------------------------

const RestrictedNaturalAction::domain_type& RestrictedNaturalAction::domain() const {
	return Omega;
}

RestrictedNaturalAction::RestrictedNaturalAction( Group G, const RestrictedNaturalAction::domain_type& S ) : PointAction<RestrictedNaturalAction,RestrictedNaturalAction::value_type,RestrictedNaturalAction::domain_type>( G ), Omega( S ) {
}

// ----------------------------------------------------------------

/*std::vector<std::vector<NaturalAction::value_type>> NaturalAction::calculateOrbits() const {
	size_t n = domain().size();
	const auto& gens = group()->generators();
	UnionFind uf( n );
	std::vector<std::vector<int>> r;
	std::map<int,int> x;
	for( size_t i = 0; i < n; i++ )
		for( const auto& g : gens )
			uf.cup( i, g(i) );
	for( size_t i = 0; i < n; i++ ) {
		int a = uf.find( i );
		if( x.count( a ) == 0 ) {
			x[a] = r.size();
			r.push_back({ int(i) });
		} else
			r[x[a]].push_back( i );
	}
	return r;
}

RestrictedNaturalSetAction NaturalAction::randomBlocksystem() const {
	size_t N = domain().size();
	size_t block_count;
	std::stack<value_type> C;
	UnionFind f( N );
	std::vector<value_type> P_omega;
	for( int omega : range( 1, N ) ) {
		C.push( omega );
		f.clear();
		f.cup( 0, omega );
		block_count = N;
		while( !C.empty() ) {
			int beta = C.top();
			C.pop();
			int alpha = f.find( beta );
			for( const auto& g : group()->generators() ) {
				int gamma = act( g, alpha );
				int delta = act( g, beta );
				if( f.find(gamma) != f.find(delta) ) {
					C.push( std::max( f.find(gamma), f.find(delta) ) );
					block_count -= f.cup( gamma, delta );
				}
			}
		}
		P_omega.resize( N / block_count );
		int j = 0;
		for( int i : domain() )
			if( f.find( i ) == 0 )
				P_omega[j++] = i;
		if( block_count != 1 )
			break;
	}
	RestrictedNaturalSetAction::domain_type block_system = NaturalSetAction( group(), N, P_omega.size() ).orbit<RestrictedNaturalSetAction::domain_type>( P_omega );
	return RestrictedNaturalSetAction( group(), std::move( block_system ) );
}

NaturalAction::value_type NaturalAction::act( const Permutation& sigma, NaturalAction::value_type x ) const {
	return sigma( x );
}*/

// ----------------------------------------------------------------

NaturalSetAction::domain_type NaturalSetAction::domain() const {
	return domain_type( subdomain().size(), k );
}

range NaturalSetAction::subdomain() const {
	return range( 0, n );
}

NaturalSetAction::NaturalSetAction( Group G, int subdomain_size, int set_size ) : SetAction<NaturalSetAction,NaturalSetAction::value_type,NaturalSetAction::domain_type>( G, set_size ) {
	n = subdomain_size;
}

// ----------------------------------------------------------------

const RestrictedNaturalSetAction::domain_type& RestrictedNaturalSetAction::domain() const {
	return Omega;
}

RestrictedNaturalSetAction::RestrictedNaturalSetAction( Group G, domain_type D ) : SetAction<RestrictedNaturalSetAction,RestrictedNaturalSetAction::value_type,RestrictedNaturalSetAction::domain_type>( G, D[0].size() ) {
	Omega = std::move( D );
}

RestrictedNaturalSetAction::RestrictedNaturalSetAction( const NaturalAction& X ) : SetAction<RestrictedNaturalSetAction,RestrictedNaturalSetAction::value_type,RestrictedNaturalSetAction::domain_type>( X.group(), 1 ) {
	for( auto x : X.domain() )
		Omega.push_back( {x} );
}


RestrictedNaturalSetAction RestrictedNaturalSetAction::randomBlocksystem() const {
	size_t N = domain().size();
	size_t block_count;
	std::stack<int> C;
	UnionFind f( N );
	std::vector<value_type> P_omega;
	std::map<value_type,int> inverse_domain;
	for( size_t i = 0; i < N; i++ )
		inverse_domain[ domain()[ i ] ] = i;
	for( int omega : range( 1, N ) ) {
		C.push( omega );
		f.clear();
		f.cup( 0, omega );
		block_count = N-1;
		while( !C.empty() ) {
			int beta = C.top();
			C.pop();
			int alpha = f.find( beta );
			for( const auto& g : group()->generators() ) {
				int gamma = inverse_domain[ act( g, domain()[alpha] ) ];
				int delta = inverse_domain[ act( g, domain()[beta] ) ];
				if( f.find(gamma) != f.find(delta) ) {
					C.push( std::max( f.find(gamma), f.find(delta) ) );
					block_count -= f.cup( gamma, delta );
				}
			}
		}
		P_omega.resize( N / block_count );
		int j = 0;
		for( size_t i = 0; i < N; i++ )
			if( f.find( i ) == 0 )
				P_omega[j++] = domain()[i];
		if( block_count != 1 )
			break;
	}
	NaturalSetAction::value_type S = flatten( P_omega );
	std::sort( S.begin(), S.end() );
	RestrictedNaturalSetAction::domain_type block_system = NaturalSetAction( group(), N, S.size() ).orbit<RestrictedNaturalSetAction::domain_type>( S );
	return RestrictedNaturalSetAction( group(), std::move( block_system ) );
}