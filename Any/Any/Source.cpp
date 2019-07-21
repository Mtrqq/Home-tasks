#include "Any.h"

#include <iostream>

int main()
{
	Any a;
	a = 2;
	std::cout << a.get<char>();
}
