#pragma once
#include <new>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>

template<typename T>
std::ostream& operator<<( std::ostream& os, const std::set<T>& s ) {
	if( s.empty() )
		os << "Ø";
	else {
		auto x = s.begin();
		os << "{" << (*x);
		while( (++x) != s.end() )
			os << "," << (*x);
		os << "}";
	}
	return os;
}

template<typename T>
std::ostream& operator<<( std::ostream& os, const std::vector<T>& s ) {
	if( s.empty() )
		os << "Ø";
	else {
		auto x = s.begin();
		os << "[" << (*x);
		while( (++x) != s.end() )
			os << "," << (*x);
		os << "]";
	}
	return os;
}

template<typename T, typename U>
std::ostream& operator<<( std::ostream& os, const std::map<T,U>& s ) {
	if( s.empty() )
		os << "Ø";
	else {
		auto x = s.begin();
		os << "{" << x->first << " -> " << x->second;
		while( (++x) != s.end() )
			os << "," << x->first << " -> " << x->second;
		os << "}";
	}
	return os;
}

template<typename U, typename V> class Either {
	int which;
	union {
		U first;
		V second;
	};
public:
	bool isFirst() const;
	bool isSecond() const;
	const U& getFirst() const;
	const V& getSecond() const;
	Either( U u );
	Either( V v );
	Either( const Either<U,V>& );
	~Either();
};

template<typename U, typename V>
bool Either<U,V>::isFirst() const {
	return which == 0;
}

template<typename U, typename V>
bool Either<U,V>::isSecond() const {
	return which == 1;
}

template<typename U, typename V>
const U& Either<U,V>::getFirst() const {
	return first;
}

template<typename U, typename V>
const V& Either<U,V>::getSecond() const {
	return second;
}

template<typename U, typename V>
Either<U,V>::Either( U u ) {
	which = 0;
	new (&first) U( u );
}

template<typename U, typename V>
Either<U,V>::Either( V v ) {
	which = 1;
	new (&second) V( v );
}

template<typename U, typename V>
Either<U,V>::Either( const Either<U,V>& other ) {
	if( other.isFirst() ) {
		which = 0;
		new (&first) U( other.getFirst() );
	} else {
		which = 1;
		new (&second) V( other.getSecond() );
	} 
}

template<typename U, typename V>
Either<U,V>::~Either() {
	if( isFirst() )
		first.~U();
	else
		second.~V();
}

template<typename T>
std::vector<T> to_vector( const std::set<T>& s ) {
	std::vector<T> r;
	r.reserve( s.size() );
	for( const auto& x : s )
		r.emplace_back( x );
	return r;
}

struct Empty {
};

std::ostream& operator<<( std::ostream& os, Empty e );

template<typename U, typename V>
std::ostream& operator<<( std::ostream& os, const Either<U,V>& e ) {
	if( e.isFirst() )
		return os << e.getFirst();
	else
		return os << e.getSecond();
}

class all_tuples {
	int _n;
	int _r;
public:
	class iterator {
		int _n;
		int _r;
		std::vector<int> _tuple;
	public:
		typedef iterator self_type;
		typedef std::vector<int> value_type;
		typedef std::vector<int>& reference;
		typedef std::vector<int>* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef size_t difference_type;
		iterator( int n, int r );
		iterator( const self_type& other );
		self_type& operator++();
		self_type operator++(int);
		reference operator*();
		pointer operator->();
		bool operator==(const self_type& rhs);
		bool operator!=(const self_type& rhs);
	};
	iterator begin();
	iterator end();
	all_tuples( int n, int r );
};

class all_ordered_tuples {
	int _n;
	int _r;
public:
	class iterator {
		int _n;
		int _r;
		std::vector<int> _tuple;
	public:
		typedef iterator self_type;
		typedef std::vector<int> value_type;
		typedef std::vector<int>& reference;
		typedef std::vector<int>* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef size_t difference_type;
		iterator( int n, int r );
		iterator( const self_type& other );
		self_type& operator++();
		self_type operator++(int);
		reference operator*();
		pointer operator->();
		bool operator==(const self_type& rhs);
		bool operator!=(const self_type& rhs);
	};
	iterator begin();
	iterator end();
	all_ordered_tuples( int n, int r );
};

int pow(int,int);

template<typename T>
std::vector<int> index_sort( const T& data ) {
	int n = data.size();
	std::vector<int> mapping( n, 0 );
	for( int i = 0; i < n; i++ )
		mapping[i] = i;
	std::sort( mapping.begin(), mapping.end(), [&data]( int lhs, int rhs ) {
			return data[lhs] < data[rhs];
		} );
	return mapping;
}

std::vector<int> inverse_mapping( const std::vector<int>& d );

template<typename T>
std::vector<int> rank( const T& data ) {
	auto indexing = index_sort( data );
	return inverse_mapping( indexing );
}

template<typename T> bool size_compare( const T& lhs, const T& rhs ) {
	return lhs.size() < rhs.size();
}