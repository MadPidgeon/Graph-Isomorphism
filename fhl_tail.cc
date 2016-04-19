#include "fhl_tail.h"


class FurstHopcroftLuksTail {
	typedef std::pair<Permutation,Permutation> team;
	Group G;
	Group H;
	std::vector<std::set<team>> reps;
	int n;
	bool filter( Permutation, bool add = true );
public:
	FurstHopcroftLuks() : G( nullptr ), H( nullptr ) {}
	void create( Group groupG, Group groupH, std::deque<team> seed );
};

void FurstHopcroftLuksTail::create( Group groupG, Group groupH, std::deque<team> seed ) {
	G = groupG;
	H = groupH;
	n = G->degree();
	reps.resize( n );
	for( auto& rep : reps )
		reps.emplace( G.one(), H.one() );
	bool change = true;
	for( auto& p : seed )
		filter( p );
	std::vector<std::vector<team>> old_pass( n );
	std::vector<std::vector<team>> new_pass;
	while( change ) {
		new_pass.resize( n );
		for( int i = 0; i < _n; i++ )
			for( int j = 0; j < n; j++ )
				for( auto a : old_pass[i] )
					for( auto b : reps[j] )
						change |= filter(make_pair(a.first*b.first,a.second*b.second));
		old_pass = std::move( new_pass );
	}
}

bool FurstHopcroftLuksTail::filter( team alpha, bool add ) {
	for( int i = 0; i < _n; i++ ) {
		bool found = false;
		for( const auto& sigma : reps[i] ) {
			team tau = make_pair( sigma.first.inverse() * alpha.first, sigma.second.inverse() * alpha.second );
			bool doesFix = true;
			for( int j = 0; j <= i; j++ ) {
				if( (tau.first)(j) != j ) {
					doesFix = false;
					break;
				}
			}
			if( doesFix ) {
				alpha = std::move( tau );
				found = true;
				break;
			}
		}
		if( !found ) {
			if( reps[i].count( alpha ) == 0 ) {
				if( add )
					reps[i].insert( alpha );
				return true;
			} else 
				return false;
		}
	}
	return false;
}
