#pragma once

#include <functional>
#include <set>
#include <vector>
#include <stack>
#include <exception>
#include <map>


template<typename T>
class _Action {
	Group _group;
public:
	typedef std::shared_pointer<const _Action<T>> Action;
	Group group() const;
	virtual size_t domain_size() const;
	virtual any_const_iterator<T> cbegin() const = 0;
	virtual any_const_iterator<T> cend() const = 0;
	virtual std::vector<std::vector<int>> orbits() const = 0;
	virtual Group anonymize() const = 0;
	virtual T operator()( const Permutation& sigma, const T& x ) const = 0;
	virtual ~_Action() = 0;
};

class NaturalAction : public _Action<int> {
public:
	typedef int value_type;
	virtual size_t domain_size() override const;
	virtual any_const_iterator<value_type> override cbegin() const;
	virtual any_const_iterator<value_type> override cend() const;
	virtual std::vector<std::vector<value_type>> override orbits() const;
	virtual Group anonymize() override const;
	virtual value_type operator()( const Permutation& sigma, const value_type& x ) override const;
	Action setAction() const;
	template<int k> Action arrayAction() const; 
};

template<size_t k>
class NaturalArrayAction : public _Action<std::array<int,k>> {
public:
	typedef std::array<int,k> value_type;
	virtual size_t domain_size() override const;
	virtual any_const_iterator<value_type> cbegin() override const;
	virtual any_const_iterator<value_type> cend() override const;
	virtual std::vector<std::vector<value_type>> orbits() override const;
	virtual Group anonymize() override const;
	virtual value_type operator()( const Permutation& sigma, const value_type& x ) override const;
	virtual T operator()( const Permutation& sigma, const T& x ) override const;
};

class NaturalSetAction : public _Action<std::vector<int>> {
	int k;
public:
	typedef std::vector<int> value_type;
	virtual size_t domain_size() override const;
	virtual any_const_iterator<std::vector<value_type>> cbegin() override const;
	virtual any_const_iterator<std::vector<value_type>> cend() override const;
	virtual std::vector<std::vector<std::vector<value_type>>> orbits() override const;
	virtual value_type operator()( const Permutation& sigma, const value_type& x ) override const;
	virtual Group anonymize() override const;
};

// ----------------------------------------------------------------

template<typename T>
bool _Action<T>::group() const {
	return _group;
}

// ----------------------------------------------------------------

virtual size_t NaturalAction::domain_size() const {
	return group()->degree();
}

virtual any_const_iterator NaturalAction::cbegin() const {
	return range::iterator(0);
}

virtual any_const_iterator NaturalAction::cend() const {
	return range::iterator( domain_size() );
}

virtual Group NaturalAction::anonymize() const {
	return group();
}

virtual std::vector<std::vector<int>> NaturalAction::orbits() const {
	size_t n = domain().size();
	const auto& gens = group()->generators();
	UnionFind uf( n );
	std::vector<std::vector<int>> r;
	std::unordered_map<int,int> x;
	for( int i = 0; i < n; i++ )
		for( const auto& g : gens )
			uf.cup( i, g(i) );
	for( int i = 0; i < n; i++ ) {
		int a = uf.find( i );
		if( x.count( a ) == 0 ) {
			x[a] = r.size();
			r.emplace_back({ i });
		} else
			r[x[a]].push_back( i );
	}
	return r;
}

virtual int NaturalAction::operator()( const Permutation& sigma, int x ) const {
	return sigma( x );
}

// ----------------------------------------------------------------

template<size_t k>
size_t NaturalArrayAction::subdomain_size() const {
	return group()->degree();
}

template<size_t k>
virtual size_t NaturalArrayAction::domain_size() const {
	return pow( group()->degree(), k );
}

template<size_t k>
virtual any_const_iterator NaturalArrayAction::cbegin() const {
	return all_arrays<k>( subdomain_size() ).begin();
}

template<size_t k>
virtual any_const_iterator NaturalArrayAction::cend() const {
	return all_arrays<k>( subdomain_size() ).end();
}

template<size_t k>
virtual std::vector<std::vector<int>> NaturalArrayAction::orbits() const {
	size_t n = subdomain_size();
	size_t k = power();
	size_t nk = domain_size();
	const auto& gens = group()->generators();
	UnionFind uf( n );
	std::vector<std::vector<std::array<int,k>>> r;
	std::array<int,k> intr;
	std::unordered_map<int,int> x;
	for( const auto& arr : all_arrays<k>( n ) )
		for( const auto& g : gens ) {
			std::transform( arr.begin(), arr.end(), intr.begin(), g )
			uf.cup( polynomial_evaluation( arr, n ), polynomial_evaluation( intr, n ) );
		}
	for( const auto& arr : all_arrays<k>( n ) ) {
		int a = uf.find( polynomial_evaluation( arr, n ) );
		if( x.count( a ) == 0 ) {
			x[a] = r.size();
			r.emplace_back({ arr });
		} else
			r[x[a]].push_back( arr );
	}
	return r;
}

template<size_t k>
virtual Group NaturalArrayAction::anonymize() const {
	int n = subdomain_size();
	int nk = domain_size();
	std::vector<Permutation> new_generators;
	new_generators.reserve( group()->generators().size() )
	for( const Permutation& g : group()->generators() ) {
		std::vector<int> permutation_map;
		permutation_map.reserve( nk )
		for( const auto& arr : *this ) {
			int v = 0;
			for( size_t j = 0; j < k; j++ )
				v = g( arr[j] ) + n*v;
			permutation_map.push_back( v );
		}
		new_generators.emplace_back( std::move( permutation_map ) );
	}
	return new Subgroup( new SymmetricGroup( nk ), new_generators ); 
}

template<size_t k>
virtual value_type NaturalArrayAction::operator()( const Permutation& sigma, const value_type& x ) const {
	value_type r;
	for( size_t i = 0; i < k; ++i )
		r[i] = sigma( x[i] );
	return r;
}

// ----------------------------------------------------------------

size_t NaturalSetAction::subdomain_size() const {
	return group()->degree();
}

virtual size_t NaturalSetAction::domain_size() const {
	return binom( subdomain_size(), k );
}

virtual any_const_iterator NaturalSetAction::cbegin() const {
	return all_ordered_tuples( subdomain_size(), k ).begin();
}

virtual any_const_iterator NaturalSetAction::cend() const {
	return all_ordered_tuples( subdomain_size(), k ).end();
}

virtual std::vector<std::vector<value_type>> NaturalSetAction::orbits() const {
	size_t n = subdomain_size();
	size_t k = power();
	size_t nk = domain_size();
	const auto& gens = group()->generators();
	UnionFind uf( n );
	std::vector<std::vector<std::array<int,k>>> r;
	std::array<int,k> intr;
	std::unordered_map<int,int> x;
	for( const auto& arr : all_arrays<k>( n ) )
		for( const auto& g : gens ) {
			std::transform( arr.begin(), arr.end(), intr.begin(), g )
			uf.cup( polynomial_evaluation( arr, n ), polynomial_evaluation( intr, n ) );
		}
	for( const auto& arr : all_arrays<k>( n ) ) {
		int a = uf.find( polynomial_evaluation( arr, n ) );
		if( x.count( a ) == 0 ) {
			x[a] = r.size();
			r.emplace_back({ arr });
		} else
			r[x[a]].push_back( arr );
	}
	return r;
}

virtual Group NaturalSetAction::anonymize() const {
	int n = subdomain_size();
	int nck = domain_size();
	std::vector<Permutation> new_generators;
	std::deque<int> order;
	std::vector<int> target;
	new_generators.reserve( group()->generators().size() );
	order.reserve( nck );
	target.reserve( k );
	for( const Permutation& g : group()->generators() ) {
		permutation_map.reserve( nck );
		for( const auto& tup : *this ) {
			target = (*this)( g, tup );
			order.push_back( polynomial_evaluation( target, n ) );
		}
		new_generators.emplace_back( std::move( index_sort( order ) ) );
	}
	return new Subgroup( new SymmetricGroup( nck ), new_generators ); 
}

virtual value_type NaturalArrayAction::operator()( const Permutation& sigma, const value_type& x ) const {
	value_type r( k );
	for( size_t i = 0; i < k; ++i )
		r[i] = sigma( x[i] );
	return r;
}