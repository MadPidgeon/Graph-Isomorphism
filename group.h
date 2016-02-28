#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <set>

class _Group;
class Subgroup;
typedef std::shared_ptr<const _Group> Group;

#include "permutation.h"
#include "coset.h"

class _Group: public std::enable_shared_from_this<const _Group> {
public:
	virtual bool contains( const Permutation& ) const = 0;
	virtual int degree() const = 0;
	virtual int order() const = 0;
	virtual std::vector<Permutation> generators() const = 0;
	Group share() const;
	Permutation one() const;
	bool hasSubgroup( Group H ) const;
	bool equals( Group H ) const;
	std::vector<int> domain() const;
	virtual ~_Group() = 0;
	std::vector<Coset> allCosets( Group G, bool right = true ) const;
protected:
	static bool _filter( Permutation alpha, std::vector<std::set<Permutation>>& reps, std::function<bool(Permutation)> c );
};

class FurstHopcroftLuks {
	const _Group* _G;
	std::vector<std::set<Permutation>> _reps;
	int _n;
	mutable int _order;
	bool filter( Permutation, bool add = true );
public:
	FurstHopcroftLuks() : _G( nullptr ), _order(0) {}
	void create( const _Group* group );
	bool contains( const Permutation& );
	int order() const;
	operator bool() const;
};

class Subgroup: public _Group {
	Group _supergroup;
	std::vector<Permutation> _generators;
	mutable FurstHopcroftLuks _fhl;
public:
	Group supergroup() const;
	virtual bool contains( const Permutation& ) const;
	virtual int degree() const;
	virtual int order() const;
	virtual std::vector<Permutation> generators() const;
	Subgroup( Group, std::vector<Permutation> );
	Subgroup( Group, std::function<bool(Permutation)> );
	virtual ~Subgroup();
};

class SymmetricGroup: public _Group {
	int _degree;
public:
	virtual bool contains( const Permutation& ) const;
	virtual int degree() const;
	virtual int order() const;
	virtual std::vector<Permutation> generators() const;
	SymmetricGroup( int n );
	virtual ~SymmetricGroup();
};