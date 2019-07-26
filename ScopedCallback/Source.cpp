#include "ScopedCallback.h"

#include <iostream>

int main()
{
	try
	{
		nostd::ScopedCaller caller{ [] { throw 1; } };
		throw 1;
		caller.Reset([] {std::cout << "Bye"; });
	}
	catch(...) { }
}