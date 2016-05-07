#include "permutation.h"
#include "fhl.h"

bool PermutationPullback::isIdentity() const {
	return original.isIdentity();
}

Permutation PermutationPullback::getPullback() const {
	return pullback.inverse();
}

int PermutationPullback::degree() const {
	return original.degree();
}

PermutationPullback PermutationPullback::inverse() const {
	return PermutationPullback( std::move( original.inverse() ), std::move( pullback.inverse() ) );
}

int PermutationPullback::operator()( int i ) const {
	return original( i );
}

PermutationPullback PermutationPullback::operator*( const PermutationPullback& other ) const {
	return PermutationPullback( std::move( original * other.original ), std::move( pullback * other.pullback ) );
}

PermutationPullback::PermutationPullback( Permutation&& o, Permutation&& p ) : original( std::move( o ) ), pullback( std::move( p ) ) {
}

PermutationPullback::PermutationPullback( Permutation p ) : original( p ), pullback( std::move( p ) ) {
}

PullbackStructure::PullbackStructure( Group pullback_space, const std::vector<PermutationPullback>& generators ) : FHL<PermutationPullback>( generators, generators.front().degree() ) {
	domain = pullback_space; 
}

PullbackStructure::PullbackStructure( Group pullback_space, std::vector<Permutation> originals, std::vector<Permutation> pullbacks ) {
	domain = pullback_space;
	std::vector<PermutationPullback> generators;
	generators.reserve( originals.size() );
	for( size_t i = 0; i < originals.size(); ++i )
		generators.emplace_back( std::move( originals[i] ), std::move( pullbacks[i] ) );
	create( generators, generators.front().degree() );
}

Permutation PullbackStructure::operator()( Permutation sigma ) const {
	return find( PermutationPullback( std::move( sigma ), std::move( domain->one() ) ) ).getPullback();
}

// --------------------------------------------------------------------------------------------------------------

Permutation SubgroupGenerator::filter( Permutation sigma, bool add ) const {
	//std::cout << sigma << std::endl;
	//std::cin.get();
	if( check( sigma ) )
		return FHL<>::filter( sigma, add );
	for( const Permutation& tau : representatives ) {
		Permutation mu( tau * sigma );
		if( check( mu ) )
			return FHL<>::filter( mu, add );
	}
	Permutation nu = sigma.inverse();
	if( add )
		representatives.push_back( nu );
	return sigma;
}

SubgroupGenerator::SubgroupGenerator( Group H, std::function<bool(Permutation)> func ) {
	create( H, func );
}

void SubgroupGenerator::clear() {
	representatives.clear();
	FHL<>::clear();
	check = nullptr;
}

void SubgroupGenerator::create( Group H, const FHL<Permutation>& P ) {
	clear();
	//*((FHL<>*) this) = P;
	G = H;
	check = [&]( const Permutation& sigma ) -> bool { return P.contains( sigma ); };
	n = G->generators().back().degree();
	m = n - 1;
	subcreate();
}

void SubgroupGenerator::subcreate() {
	auto generators = G->generators();
	std::cerr << generators << std::endl;
	if( n > 0 ) {
		V.resize( m );
		std::cerr << " a";
		for( size_t i = 0; i < m; ++i )
			V[i].resize( n - i - 1, Permutation( 0 ) );
		std::cerr << "b";
		std::deque<Permutation> new_permutations;
		for( auto sigma : generators )
			new_permutations.push_back( filter( sigma, true ) );
		std::cerr << "c";
		// std::cout << new_permutations << std::endl;
		while( not new_permutations.empty() ) {
			std::cout << V << representatives << std::endl;
			Permutation sigma = std::move( new_permutations.front() );
			Permutation mu( 0 );
			new_permutations.pop_front();
			for( const auto& W : V ) {
				for( const auto& tau : W ) {
					if( tau.degree() > 0 ) {
						Permutation nu = tau.inverse();
						mu = filter( sigma * nu, true );
						if( not mu.isIdentity() )
							new_permutations.push_back( std::move( mu ) );
						mu = filter( nu * sigma, true );
						if( not mu.isIdentity() )
							new_permutations.push_back( std::move( mu ) );
					}
				}
				for( const auto& tau : representatives ) {
					Permutation nu = tau.inverse();
					mu = filter( sigma * nu, true );
					if( not mu.isIdentity() )
						new_permutations.push_back( std::move( mu ) );
					mu = filter( nu * sigma, true );
					if( not mu.isIdentity() )
						new_permutations.push_back( std::move( mu ) );
				}
			}
		}
	}
}

void SubgroupGenerator::create( Group H, std::function<bool(Permutation)> func ) {
	clear();
	G = H;
	check = func;
	n = G->generators().back().degree();
	m = n - 1;
	subcreate();
}

bool SubgroupGenerator::contains( const Permutation& sigma ) const {
	return filter( sigma, false ).isIdentity();
}

Permutation SubgroupGenerator::find( const Permutation& sigma ) const {
	return filter( sigma, false );
}

Subgroup SubgroupGenerator::subgroup() const {
	return Subgroup( G, listGenerators() );
}

std::deque<Permutation> SubgroupGenerator::cosetRepresentatives() const {
	std::deque<Permutation> r = representatives;
	r.push_back( G->one() );
	return r;
}