#include <iostream>
#include "ext.h"

int main() {
	for( int x : range(0,20) )
		std::cout << x << std::endl;
	std::cout << "----------" << std::endl;
	for( const auto& tuple : all_tuples( 5, 3 ) )
		std::cout << tuple << std::endl;
	std::cout << "----------" << std::endl;
	for( const auto& set : all_ordered_tuples( 5, 3 ) )
		std::cout << set << std::endl;
	std::cout << "----------" << std::endl;
	return 0;
}