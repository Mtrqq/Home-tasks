#include "Variant.h"

#include <iostream>

class A {};

// add enable if
// add some functions tests
// incapsulate implementation structs

int main()
{
	Variant<int, double, char> var{ '1' };
	
	std::cout << var.get<2>() << std::endl;
}