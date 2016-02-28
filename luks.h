#pragma once

#include <string>
#include <vector>

#include "action.h"
#include "group.h"
#include "coset.h"
#include "ext.h"

Either<Coset,Empty> LuksChainRule( Group G, std::vector<std::set<int>> windows, std::string x, std::string y, int depth );


std::string LuksStringAction( const Permutation& sigma, std::string x );
std::string LuksStringAction( const Permutation& sigma, std::string x, const std::vector<int>& window );
std::string LuksStringAction( const Permutation& sigma, std::string x, const std::set<int>& window );

Either<Coset,Empty> LuksStringIsomorphism( Group G, std::set<int> window, std::string x, std::string y, int depth );