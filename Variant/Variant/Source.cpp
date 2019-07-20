#include "Variant.h"

#include <iostream>

int main()
{
	std::ios::sync_with_stdio(false);
	Variant<int, double, char> var{ '1' };
	std::cout << var.index() << '\t' << var.get<char>() << std::endl;
	var = 2.0;
	try 
	{
		std::cout << var.get<char>() << std::endl;
	}
	catch (const std::bad_typeid&)
	{
		std::cout << "OK\n";
	}
	std::cout << var.index() << '\t' << var.get<1>() << std::endl;
	const Variant<int, double, char> test = '5'; 
	std::cout << test.get<char>();
	/*Variant<int, double, char> copy_of_var{ var }; Error
	 it tries to call Variant (T&& i_value) instead of Variant(const Variant<Args...>& another)
	 how to fix it ? */
}
