#include "Vector.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std::chrono;

template <class Container>
void vectorTest()
{
	Container values;
	values.reserve(1000000);
	for (unsigned i = 0; i < 1000000; ++i)
	{
		values.push_back(1);
	}
	unsigned accum{};
	for (unsigned i = 0; i < 1000000; ++i)
	{
		accum += values[i];
	}
	while (!values.empty()) values.pop_back();
	//std::cout << "\nAccumulator equals : " << accum << std::endl;
}

template <class Container, typename T>
void functionsTest(std::initializer_list<T> &&init)
{
	Container container = std::move(init);
	auto lastElem = container.back();
	std::cout << "Last element = " << lastElem << std::endl
		<< "First element = " << container.front() << std::endl
		<< "isEmpty : " << container.empty() << std::endl;
	container.push_back(T());
	container.pop_back();
	std::cout << "Valid insertion : " << (lastElem == container.back()) << std::endl
		<< "Size : " << container.size() << '\t' << "Capacity : " << container.capacity() << "\n\n\n";
}

int main()
{
	double std_vector_accumulator{}, nostd_vector_accumulator{};
	unsigned amount_of_tests{100};
	for (unsigned i = 0; i < amount_of_tests; ++i)
	{
		auto start = steady_clock::now();
		vectorTest<std::vector<unsigned>>();
		std_vector_accumulator += duration<double>(steady_clock::now() - start).count();
		start = steady_clock::now();
		vectorTest<nostd::Vector<unsigned>>();
		nostd_vector_accumulator += duration<double>(steady_clock::now() - start).count();
	}
	std::cout << "std::vector : " << std_vector_accumulator / amount_of_tests << std::endl;
	std::cout << "nostd::Vector : " << nostd_vector_accumulator / amount_of_tests << "\n\n\n";
	functionsTest<std::vector<int>>({ 1,2,3,4,5 });
	functionsTest<nostd::Vector<int>>({ 1,2,3,4,5 });

	std::cout << "\nEmplace test : \n";
	nostd::Vector<std::string> strVec;
	strVec.emplace_back(10, 'q');
	std::cout << strVec.front();
}