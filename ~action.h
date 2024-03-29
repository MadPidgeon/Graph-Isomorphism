#pragma once

#include <functional>
#include <set>
#include <vector>
#include <stack>
#include <exception>
#include <map>

template<typename T> class Action;

#include "permutation.h"
#include "group.h"
#include "unionfind.h"
#include "matrix.h"
#include "ext.h"

template<typename T> 
class Action {
public:
	typedef std::function<T(const Permutation&,T)> actionFunction;

private:
	Group _G;
	actionFunction _action;
	std::vector<T> _domain;
	mutable std::vector<std::vector<T>> _orbits;
	mutable Group _kernel;

public:
	T operator()( const Permutation& sigma, T x ) const;

	const std::vector<T>& domain() const;
	Group group() const;
	std::vector<std::vector<T>> orbits() const;
	std::vector<std::vector<std::array<T,2>>> orbitals() const;
	std::vector<std::vector<std::array<T,2>>> hack__orbitals() const;
	actionFunction function() const;
	//Group stabilizer() const;
	Group kernel() const;

	bool isTransitive() const;
	bool isTrivial() const;

	Action<T> subgroupAction( Group G ) const;
	Action<std::set<T>> setwiseAction( std::vector<std::set<T>> ) const;
	Action<std::set<T>> setwiseAction( int t ) const;
	Action<std::vector<T>> tupleAction( int k ) const;
	template<int k>
	Action<std::array<T,k>> arrayAction() const;

	Action<std::set<T>> randomBlocksystem() const;
	Action<std::set<T>> reverseSystemOfImprimitivity() const;
	Action<std::set<T>> systemOfImprimitivity() const;

	Group anonymize() const;
	
	Action( Group G, std::vector<T> domain, actionFunction action );
};

Action<int> SubsetAction( Group G, std::vector<int> subset );
Action<int> NaturalAction( Group G );
//Action<matrix<int>> MatrixAction( Group G );

template<typename T>
Action<T> ActionOnTransitiveClosure( Group G, T x, typename Action<T>::actionFunction _action ) {
	std::set<T> domain;
	std::stack<T> s;
	s.push( x );
	while( !s.empty() ) {
		T y = s.top();
		s.pop();
		for( const auto& gen : G->generators() ) {
			T z = _action( gen, y );
			if( domain.count(z) == 0 ) {
				domain.insert(z);
				s.push(z);
			}
		}
	}
	std::vector<T> vector_domain;
	std::copy( domain.begin(), domain.end(), std::back_inserter( vector_domain ) );
	return Action<T>( G, vector_domain, _action );
}

template<typename T>
T Action<T>::operator()( const Permutation& sigma, T x ) const {
	/*if( ! _G->contains( sigma ) )
		throw std::range_error( "Permutation not part of group action" );*/
	return _action( sigma, x );
}

template<typename T>
const std::vector<T>& Action<T>::domain() const {
	return _domain;
}

template<typename T>
Group Action<T>::group() const {
	return _G;
}

template<typename T>
std::vector<std::vector<T>> Action<T>::orbits() const {
	if( !_orbits.empty() )
		return _orbits;
	std::set<T> done;
	std::stack<T> to_do;
	std::vector<T> o;
	auto gens = _G->generators();
	int counter = 0;
	T y;
	// improve with union find
	for( auto x : _domain ) {
		if( done.count( x ) == 0 ) {
			to_do.push( x );
			while( !to_do.empty() ) {
				if( (++counter) % 10000 == 0 )
					std::cout << counter << std::endl;
				y = to_do.top();
				to_do.pop();
				if( done.count( y ) == 0 ) {
					done.insert( y );
					o.push_back( y );
					for( auto& gen : gens ) {
						to_do.push( (*this)( gen, y ) );
					}
				}
			}
			_orbits.emplace_back( std::move( o ) );
		}
	}
	return _orbits;
}

template<typename T>
std::vector<std::vector<std::array<T,2>>> Action<T>::orbitals() const {
	return arrayAction<2>().orbits();
}

template<typename T>
std::vector<std::vector<std::array<T,2>>> Action<T>::hack__orbitals() const {
	int n = domain().size();
	const auto& gens = group()->generators();
	UnionFind uf( n*n );
	std::vector<std::vector<std::array<T,2>>> r;
	std::map<T,T> x;
	for( int i = 0; i < n; i++ )
		for( int j = 0; j < n; j++ )
			for( const auto& g : gens )
				uf.cup( i*n+j, g(i)*n+g(j) );
	for( int i = 0; i < n; i++ ) {
		for( int j = 0; j < n; j++ ) {
			T a = uf.find( i*n+j );
			if( x.count(a) == 0 ) {
				x[a] = r.size();
				r.emplace_back();
			}
			r[x[a]].push_back(std::array<T,2>({i,j}));
		}
	}
	return r;
}

template<typename T>
bool Action<T>::isTransitive() const {
	return orbits().size() == 1;
}

template<typename T>
bool Action<T>::isTrivial() const {
	return domain().size() == 1;
}

template<typename T>
typename Action<T>::actionFunction Action<T>::function() const {
	return _action;
}


template<typename T>
Action<std::set<T>> Action<T>::systemOfImprimitivity() const {
	std::vector<std::set<T>> set_domain;
	for( auto& x : _domain )
		set_domain.push_back( std::set<T>( { x } ) );
	Action<std::set<T>> soi = setwiseAction( set_domain );
	do {
		Action<std::set<std::set<T>>> next_soi = soi.randomBlocksystem();
		if( next_soi.isTrivial() )
			return soi;
		set_domain.clear();
		for( auto& X : next_soi.domain() ) {
			std::set<T> Y;
			for( auto& x : X )
				Y.insert( x.begin(), x.end() );
			set_domain.push_back( Y );
		}
		soi = setwiseAction( set_domain );
	} while( true );
}

template<typename T>
Action<std::set<T>> Action<T>::randomBlocksystem() const {
	// M. D. Atkinson
	size_t N = _domain.size();
	std::map<T,int> inverse_domain;
	std::stack<int> C;
	UnionFind f( N );
	for( int i = 0; i < N; i++ )
		inverse_domain[ _domain[ i ] ] = i;
	std::map<int,int> other_map;
	std::set<T> P_omega;
		
	for( int omega = 1; omega < N; omega++ ) {
		C.push( omega );
		f.clear();
		f.cup( 0, omega );
		while( !C.empty() ) {
			int beta = C.top();
			C.pop();
			int alpha = f.find( beta );
			for( const auto& gen : _G->generators() ) {
				int gamma = inverse_domain[ _action( gen, _domain[ alpha ] ) ];
				int delta = inverse_domain[ _action( gen, _domain[ beta ] ) ];
				if( f.find(gamma) != f.find(delta) ) {
					C.push( std::max( f.find(gamma), f.find(delta) ) );
					f.cup( gamma, delta );
				}
			}
		}

		P_omega.clear();
		other_map.clear();
		for( int i = 0; i < N; i++ )
			if( f.find( i ) == 0 )
				P_omega.insert( _domain[i] );

		if( P_omega.size() != N ) {
			//std::cerr << "Found!" << std::endl;
			break;
		} /*else 
			std::cerr << "X";*/
	}

	auto a = _action;
	return ActionOnTransitiveClosure<std::set<T>>( _G, P_omega, 
		[a]( const Permutation& sigma, std::set<T> X ) -> std::set<T> { 
			std::set<T> r; 
			for( auto& x : X ) 
				r.insert( a( sigma, x ) );
			return r;
		} );
}

template<typename T>
Action<std::set<T>> Action<T>::reverseSystemOfImprimitivity() const {
	// M. D. Atkinson
	size_t N = _domain.size();
	std::map<T,int> inverse_domain;
	std::stack<int> C;
	UnionFind f( N );
	for( int i = 0; i < N; i++ )
		inverse_domain[ _domain[ i ] ] = i;
	std::map<int,int> other_map;
	std::set<T> P_omega;
	std::set<T> best;
	for( int omega = 1; omega < N; omega++ ) {
		C.push( omega );
		f.clear();
		f.cup( 0, omega );
		while( !C.empty() ) {
			int beta = C.top();
			C.pop();
			int alpha = f.find( beta );
			for( const auto& gen : _G->generators() ) {
				int gamma = inverse_domain[ _action( gen, _domain[ alpha ] ) ];
				int delta = inverse_domain[ _action( gen, _domain[ beta ] ) ];
				if( f.find(gamma) != f.find(delta) ) {
					C.push( std::max( f.find(gamma), f.find(delta) ) );
					f.cup( gamma, delta );
				}
			}
		}

		P_omega.clear();
		other_map.clear();
		for( int i = 0; i < N; i++ )
			if( f.find( i ) == 0 )
				P_omega.insert( _domain[i] );

		if( 1 < P_omega.size() && ( P_omega.size() < best.size() || best.size() == 0 ) )
			best = std::move( P_omega );
	}
	if( best.size() == 0 )
		throw std::range_error( "Group action is primitive" );

	auto a = _action;
	return ActionOnTransitiveClosure<std::set<T>>( _G, best, 
		[a]( const Permutation& sigma, std::set<T> X ) -> std::set<T> { 
			std::set<T> r; 
			for( auto& x : X ) 
				r.insert( a( sigma, x ) );
			return r;
		} );
}

template<typename T>
Action<T> Action<T>::subgroupAction( Group G ) const {
	if( !_G->hasSubgroup( G ) )
		throw std::range_error( "Group is not a subgroup" );
	return Action<T>( G, _domain, _action );
}

template<typename T>
Action<std::set<T>> Action<T>::setwiseAction( std::vector<std::set<T>> d ) const {
	actionFunction a = _action;
	return Action<std::set<T>>( _G, d, [a]( const Permutation& sigma, std::set<T> X ) { 
		std::set<T> Y; 
		for( auto& x : X ) 
			Y.insert( a( sigma, x ) ); 
		return Y; 
	} );
}

template<typename T>
Action<std::vector<T>> Action<T>::tupleAction( int k ) const {
	std::cerr << "A";
	std::vector<std::vector<T>> new_domain;
	for( auto& tup : all_tuples( _domain.size(), k ) ) {
		std::vector<T> new_tuple;
		new_tuple.reserve( k );
		for( int x : tup )
			new_tuple.push_back( _domain[x] );
		new_domain.emplace_back( std::move( new_tuple ) );
	}
	std::cerr << "B";
	actionFunction a = _action;
	return Action<std::vector<T>>( _G, new_domain, [a]( const Permutation& sigma, const std::vector<T>& X ) { 
		std::vector<T> Y;
		Y.reserve( X.size() ); 
		for( auto& x : X ) 
			Y.push_back( a( sigma, x ) ); 
		return Y; 
	} );
}

template<typename T>
template<int k>
Action<std::array<T,k>> Action<T>::arrayAction() const {
	std::cerr << "A";
	std::vector<std::array<T,k>> new_domain;
	for( auto& tup : all_tuples( _domain.size(), k ) ) {
		new_domain.emplace_back();
		for( size_t i = 0; i < k; i++ )
			new_domain.back()[i] = _domain[tup[i]];
	}
	std::cerr << "B";
	actionFunction a = _action;
	return Action<std::array<T,k>>( _G, new_domain, [a]( const Permutation& sigma, const std::array<T,k>& X ) { 
		std::array<T,k> Y;
		for( size_t i = 0; i < k; i++ )
			Y[i] = a( sigma, X[i] );
		return Y; 
	} );
}

template<typename T>
Group Action<T>::anonymize() const {
	int n = domain().size();
	Group S( new SymmetricGroup( n ) );
	std::map<T,int> inverse_mapping;
	for( int i = 0; i < n; i++ )
		inverse_mapping[_domain[i]] = i;
	std::vector<Permutation> generators;
	for( const Permutation& sigma : group()->generators() ) {
		std::vector<int> generator(n);
		for( int i = 0; i < n; i++ )
			generator[i] = inverse_mapping[_action(sigma,_domain[i])];
		generators.emplace_back( std::move( generator ) );
	}
	return Group( new Subgroup( S, generators ) );
}

/*
template<typename T>
Group Action<T>::stabilizer() const {
	return group();
}*/

template<typename T>
Action<T>::Action( Group G, std::vector<T> domain, actionFunction action ) {
	_G = std::move( G );
	_domain = std::move( domain );
	_action = std::move( action );
}

template<typename T>
Group Action<T>::kernel() const {
	if( _kernel )
		return _kernel;
	const std::vector<T>& d = domain();
	const actionFunction& a = _action;
	_kernel.reset( new Subgroup( group(), [a,d](const Permutation& sigma)->bool { 
		for( const auto& x : d ) 
			if( a( sigma, x ) != x ) 
				return false; 
		return true; 
	} ) );
	return _kernel;
}