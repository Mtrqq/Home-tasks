#include "SharedPointer.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>

std::shared_ptr<int> s;

int main()
{
	nostd::SharedPointer<int> s = nostd::make_shared<int>(123456789);
	std::vector<nostd::SharedPointer<int>> vec;
	vec.reserve(100);
	for (int i = 0; i < 100; ++i)
	{
		vec.push_back(nostd::SharedPointer<int>{s});
	}
	std::cout << s.uses_count() << std::endl;
	vec.resize(50);
	std::cout << s.uses_count() << std::endl;
	vec.clear();
	nostd::WeakPointer<int> weak = s;
	std::cout.setf(std::ios::boolalpha);
	std::cout << "Is valid :" << s.is_valid() << ",\tvalue: " << *s << std::endl;
	s.reset();
	std::cout << "Is valid after uses_count = 0 : " << s.is_valid() << std::endl;
	std::cout << "Shared Pointers Realeased" << std::endl;
	system("pause");
}