#include "function.h"

#include <iostream>

int qqq()
{
	return 1;
}

struct Functor
{
	int operator()()
	{
		return 3;
	}
};

int main()
{
	Function<int()> fn = qqq;
	std::cout << fn() << std::endl;
	std::cout << fn.target_type().name() << std::endl;
	auto lambda = [] {return 2; };
	fn = lambda;
	std::cout << fn() << std::endl;
	std::cout << fn.target_type().name() << std::endl;
	Functor functor;
	fn = functor;
	std::cout << fn() << std::endl;
	std::cout << fn.target_type().name() << std::endl;
	Function<int()> anotherFunction = fn;
	std::cout << anotherFunction() << std::endl;
	std::cout << anotherFunction.target_type().name() << std::endl;
	std::cin.get();
}