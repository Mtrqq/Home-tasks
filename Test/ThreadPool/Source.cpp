#include "ThreadPool.h"

#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <chrono>

template <typename FunctionType>
std::vector<std::future<double>> GenerateValues(unsigned amount_of_actions, FunctionType function)
{
	std::vector<std::future<double>> futures;
	futures.reserve(amount_of_actions);
	MThreadPool thread_pool;
	for (unsigned i = 0; i < amount_of_actions; ++i)
	{
		futures.push_back(thread_pool.Execute(function));
		std::this_thread::sleep_for(std::chrono::milliseconds{ 20 });
	}
	return futures;
}


int main()
{
	std::ios::sync_with_stdio(false);
	std::mt19937 generator(std::random_device{}());
	std::uniform_real_distribution<> u_rd{ 0, 10000 };
	std::vector<double> values(10'000'000);
	std::generate(values.begin(), values.end(), [&] { return u_rd(generator); });

	auto accumulator = [&values]
	{
		double variable{};
		for (unsigned i = 0; i < 10'000'000; ++i)
		{
			variable += std::log10(values[i]);
		}
		return variable;
	};

	constexpr auto useless_operations_count = 20;
	auto futures = GenerateValues(useless_operations_count, accumulator);
	std::vector<double> result;
	result.reserve(useless_operations_count);
	for (auto& future : futures)
		result.push_back(future.get());
	std::cout << "\nValues :\n";
	std::copy(result.cbegin(), result.cend(), std::ostream_iterator<double>{std::cout, "\n"});
	for (unsigned i = 1; i < useless_operations_count; ++i)
	{
		assert(result[i] == result[i - 1]); // Comparing floating point values ?
	}
	std::cout << "\nAll values are equal !" << std::endl;

	std::cin.get();
}