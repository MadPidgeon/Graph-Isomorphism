#include <iostream>
#include <vector>
#include <cmath>
#include <exception>

#include "group.h"
#include "permutation.h"
#include "action.h"

Permutation _Group::one() const {
	int n = degree();
	std::vector<int> o( n );
	for( int i = 0; i < n; i++ )
		o[i] = i;
	return Permutation( std::move(o) );
}

bool _Group::hasSubgroup( Group H ) const {
	for( const auto& gen : H->generators() )
		if( !contains( gen ) )
			return false;
	return true;
}

std::vector<int> _Group::domain() const {
	std::vector<int> d( degree() );
	for( int i = 0; i < degree(); i++ )
		d[i] = i;
	return d;
}

Group _Group::share() const {
	return shared_from_this();
}

_Group::~_Group() {
}

bool Subgroup::contains( const Permutation& alpha ) const {
	#warning ("Using membership testing")
	if( !_fhl )
		_fhl.create( this );
	return _fhl.contains( alpha );
}

bool _Group::_filter( Permutation alpha, std::vector<std::set<Permutation>>& reps, std::function<bool(Permutation)> c ) {
	for( size_t i = 0; i < reps.size(); i++ ) {
		bool found = false;
		for( const auto& sigma : reps[i] ) {
			Permutation tau = sigma.inverse() * alpha;
			bool doesFix = true;
			if( c( tau ) )
				for( size_t j = 1; j <= i; j++ ) {
					if( tau( j-1 ) != int(j)-1 ) {
						doesFix = false;
						break;
					}
				}
			else
				doesFix = false;
			if( doesFix ) {
				alpha = std::move( tau );
				found = true;
				break;
			}
		}
		if( !found ) {
			if( reps[i].count( alpha ) == 0 ) {
				reps[i].insert( alpha );
				return true;
			} else 
				return false;
		}
	}
	return false;
}


Subgroup::Subgroup( Group G, std::vector<Permutation> gens ) {
	/*for( auto& gen : gens )
		if( !G->contains( gen ) )
			throw std::range_error( "Permutation not an element of the supergroup" );*/
	swap( _supergroup, G );
	swap( _generators, gens );
}

Subgroup::Subgroup( Group G, std::function<bool(Permutation)> c ) {
	swap( _supergroup, G );
	int n = supergroup()->domain().size();
	std::vector<std::set<Permutation>> reps( n );
	for( auto& rep : reps )
		rep.insert( supergroup()->one() );
	for( auto gen : supergroup()->generators() )
		_filter( gen, reps, c );
	bool change = true;
	while( change ) {
		change = false;
		//int q = 0;
		//std::cout << "!" << std::endl;
		for( int i = 0; i < n; i++ )
			for( int j = 0; j <= i; j++ )
				for( const auto& a : reps[i] ) // filter will not add to i or j, so no invalidation 
					for( const auto& b : reps[j] ) {
						/*std::cout << a << "~" << b << std::endl;
						for( int q = 0; q < reps.size(); q++ )
							std::cout << "Q" << q << ": " << reps[q] << std::endl;*/
						/*if( (q++) % 10000 == 0 )
							std::cout << reps[0].size() << std::endl;*/
						change |= _filter( a*b, reps, c );
					}
	}
	for( int i = 1; i < n; i++ ) {
		_generators.reserve( _generators.size() + reps[i].size() );
		for( const auto& sigma : reps[i] )
			if( sigma != supergroup()->one() )
				_generators.emplace_back( sigma );
	}
}

std::vector<Coset> _Group::allCosets( Group N, bool right ) const {
	int n = domain().size();
	std::vector<std::set<Permutation>> reps( n );
	std::function<bool(const Permutation&)> c = std::bind( &_Group::contains, N.get(), std::placeholders::_1 );
	for( auto& rep : reps )
		rep.insert( one() );
	for( auto gen : generators() )
		_filter( gen, reps, c );
	bool change = true;
	while( change ) {
		change = false;
		for( int i = 0; i < n; i++ )
			for( int j = 0; j <= i; j++ )
				for( const auto& a : reps[i] ) // filter will not add to i or j, so no invalidation 
					for( const auto& b : reps[j] )
						change |= _filter( a*b, reps, c );
	}
	std::vector<Coset> cosets;
	cosets.reserve( reps[0].size() );
	for( const auto& x : reps[0] )
		cosets.emplace_back( share(), N,  x, right );
	return cosets;
}

Subgroup::~Subgroup() {
}

Group Subgroup::supergroup() const {
	return _supergroup;
}

std::vector<Permutation> Subgroup::generators() const {
	return _generators;
}

int Subgroup::degree() const {
	return supergroup()->degree();
}

int Subgroup::order() const {
	if( !_fhl )
		_fhl.create( this );
	return _fhl.order();
}

bool SymmetricGroup::contains( const Permutation& sigma ) const {
	return degree() == sigma.degree();
}

int SymmetricGroup::degree() const {
	return _degree;
}

int SymmetricGroup::order() const {
	return std::tgamma( _degree + 1 );
}

std::vector<Permutation> SymmetricGroup::generators() const {
	std::vector<int> cycle( degree() );
	std::vector<int> transposition( degree() );
	for( int i = 0; i < degree(); i++ ) {
		cycle[i] = (i+1) % degree();
		transposition[i] = i;
	}
	Permutation sigma( std::move( cycle ) );
	if( degree() <= 2 )
		return std::vector<Permutation>({ sigma });
	std::swap( transposition[0], transposition[1] );
	Permutation tau( std::move( transposition ) );
	return std::vector<Permutation>({ sigma, tau });
}

SymmetricGroup::SymmetricGroup( int n ) {
	_degree = n;
}

SymmetricGroup::~SymmetricGroup() {
}

void FurstHopcroftLuks::create( const _Group* group ) {
	if( _G || !group )
		throw std::runtime_error( "FHL structure already created" );
	_G = group;
	_n = _G->degree() - 1;
	_reps.resize( _n );
	for( auto& rep : _reps )
		rep.insert( _G->one() );
	for( auto gen : _G->generators() )
		filter( gen );
	bool change = true;
	while( change ) {
		change = false;
		/*for( auto& rep : _reps ) {
			std::cout << "-----" << std::endl;
			for( auto& sigma : rep )
				std::cout << sigma.getCycleNotation() << std::endl;
		}
		std::cin.get();*/
		/*int q = 0;
		for( const auto& rep: _reps )
			std::cout << (q++) << ": " << rep << std::endl;*/

		for( int i = 0; i < _n; i++ )
			for( int j = 0; j <= i; j++ )
				for( auto a : _reps[i] )
					for( auto b : _reps[j] )
						change |= filter( a*b );
	}
}

void FurstHopcroftLuks::create( const std::deque<Permutation>& L ) {
	if( L.size() == 0 )
		throw;
	_n = L.front().degree();
	_reps.resize( _n );
	for( auto& rep : _reps )
		rep.insert( _G->one() );
	for( const auto& sigma : L )
		filter( sigma );
	_G = new Subgroup( Group( new SymmetricGroup( _n ) ), generators() );
}

bool FurstHopcroftLuks::filter( Permutation alpha, bool add ) {
	for( int i = 0; i < _n; i++ ) {
		bool found = false;
		for( const auto& sigma : _reps[i] ) {
			Permutation tau = sigma.inverse() * alpha;
			bool doesFix = true;
			for( int j = 0; j <= i; j++ )
				if( tau(j) != j ) {
					doesFix = false;
					break;
				}
			if( doesFix ) {
				alpha = std::move( tau );
				found = true;
				break;
			}
		}
		if( !found ) {
			if( _reps[i].count( alpha ) == 0 ) {
				if( add )
					_reps[i].insert( alpha );
				return true;
			} else 
				return false;
		}
	}
	return false;
}

std::vector<Permutation> FurstHopcroftLuks::generators() const {
	std::vector<Permutation> r;
	for( int i = 0; i < _n; i++ )
		for( const auto& sigma : _reps[i] )
			if( !sigma.isIdentity() )
				r.push_back( sigma );
	return r;
}

bool FurstHopcroftLuks::contains( const Permutation& alpha ) {
	return !filter( alpha, false );
}

int FurstHopcroftLuks::order() const {
	
	if( !_order ) {
		_order = 1;
		for( const auto& rep : _reps )
			_order *= rep.size();
	}
	return _order;
}

FurstHopcroftLuks::operator bool() const {
	return _G;
}