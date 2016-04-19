#include <iostream>
#include "permutation.h"
#include "action.h"
#include "group.h"
#include "johnson.h"
#include "configuration.h"
#include "align.h"


int main() {
	int n = 5;
	Group Sn( new SymmetricGroup( n ) );
	NaturalSetAction a( Sn, Sn->degree(), 2 );
	std::map<std::vector<int>,int> reverse_anonymize;
	Group G = a.anonymize( &reverse_anonymize );
	std::cout << G->generators() << std::endl;
	std::cout << "---" << std::endl;
	std::cout << reverse_anonymize << std::endl;
	std::cout << "---" << std::endl;
	NaturalAction G_action( G );
	auto blocks = G_action.systemOfImprimitivity();
	std::cout << blocks.domain() << std::endl;
	std::cout << "---" << std::endl;
	// do cameron magic
	std::deque<std::vector<int>> Gamma;
	for( auto x : range(0,n) ) {
		std::vector<int> v;
		for( auto t : a.domain() ) {
			if( std::find( t.begin(), t.end(), x ) != t.end() )
				v.push_back( reverse_anonymize[t] );
		}
		std::sort( v.begin(), v.end() );
		Gamma.emplace_back( std::move( v ) );
	}
	std::cout << Gamma << std::endl;
	RestrictedNaturalSetAction Gamma_action( G, Gamma );
	//auto johnson_blocks = blocks;
	//auto Sk = johnson_blocks.anonymize();
	//std::cout << johnson_blocks.domain() << std::endl;
	std::cout << "---" << std::endl;
	std::map<std::vector<int>,int> reverse_anonymize2;
	Group H = Gamma_action.anonymize( &reverse_anonymize2 );

	NaturalAction H_action( H );
	std::cout << reverse_anonymize2 << std::endl;
	std::cout << "---" << std::endl;
	std::cout << H->generators() << std::endl;
	std::cout << "---" << std::endl;
	auto standard_blocks = JohnsonStandardBlocks( Gamma_action );
	std::cout << standard_blocks.domain() << std::endl;

	std::cout << "-------------------------------" << std::endl;
	RelationalStructure conf1(
		std::vector<std::set<std::vector<int>>>({
			std::set<std::vector<int>>({
				std::vector<int>({0,0}),std::vector<int>({1,1}),std::vector<int>({2,2}),std::vector<int>({3,3}),std::vector<int>({4,4})
			}),
			std::set<std::vector<int>>({
				std::vector<int>({0,1}),std::vector<int>({1,0}),
				std::vector<int>({1,3}),std::vector<int>({3,1}),
				std::vector<int>({0,4}),std::vector<int>({4,0}),
				std::vector<int>({2,4}),std::vector<int>({4,2})
			}),
			std::set<std::vector<int>>({
				std::vector<int>({1,2}),std::vector<int>({2,1}),
				std::vector<int>({1,4}),std::vector<int>({4,1}),
				std::vector<int>({3,4}),std::vector<int>({4,3}),
				std::vector<int>({0,2}),std::vector<int>({2,0})
			}),
			std::set<std::vector<int>>({
				std::vector<int>({0,3}),std::vector<int>({3,0}),
				std::vector<int>({2,3}),std::vector<int>({3,2})
			})
		})
	, 5 );
	conf1.WeisfellerLeman();
	std::cout << conf1.relations() << std::endl;
	std::cout << "-------------------------------" << std::endl;
	RelationalStructure conf2(
		std::vector<std::set<std::vector<int>>>({
			std::set<std::vector<int>>({
				std::vector<int>({0,0}),std::vector<int>({1,1}),std::vector<int>({2,2}),std::vector<int>({3,3}),std::vector<int>({4,4})
			}),
			std::set<std::vector<int>>({
				std::vector<int>({0,1}),std::vector<int>({1,0}),
				std::vector<int>({0,2}),std::vector<int>({2,0}),
				std::vector<int>({0,4}),std::vector<int>({4,0}),
				std::vector<int>({1,2}),std::vector<int>({2,1})		
			}),
			std::set<std::vector<int>>({
				std::vector<int>({1,3}),std::vector<int>({3,1}),
				std::vector<int>({2,4}),std::vector<int>({4,2}),
				std::vector<int>({1,4}),std::vector<int>({4,1}),
				std::vector<int>({3,4}),std::vector<int>({4,3})
				
			}),
			std::set<std::vector<int>>({
				std::vector<int>({0,3}),std::vector<int>({3,0}),
				std::vector<int>({2,3}),std::vector<int>({3,2})
			})
		})
	, 5 );
	conf2.WeisfellerLeman();
	std::cout << conf2.relations() << std::endl;
	std::cout << "-------------------------------" << std::endl;
	FurstHopfcroftLuksTail fhlt;
	std::deque<std::pair<Permutation,Permutation>> generator_mapping;
	fhlt.create( H, G,  )

	//Permutation sigma = pullback( H->generators()[0], reverse_anonymize2, Gamma_action.domain() );
	//std::cout << sigma << std::endl;

	return 0;
}