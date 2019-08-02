#include "ThreadPool.h"

#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <chrono>


int main()
{
	std::ios::sync_with_stdio(false);
	std::mt19937 generator(std::random_device{}());
	std::uniform_real_distribution<> u_rd{ 0, 10000 };
	std::vector<double> values(10'000'000);
	std::generate(values.begin(), values.end(), [&] { return u_rd(generator); });

	std::vector<std::future<double>> futures;
	unsigned count_of_useless_operations{ 20 };
	futures.reserve(count_of_useless_operations);
	{
		MThreadPool thread_pool;

		auto accumulator = [&values]
		{
			double variable{};
			for (unsigned i = 0; i < 10'000'000; ++i)
			{
				variable += std::log10(values[i]);
			}
			return variable;
		};

		for (unsigned i = 0; i < count_of_useless_operations; ++i)
		{
			futures.push_back(thread_pool.Execute(accumulator));
		}
		std::vector<double> values;
		values.reserve(count_of_useless_operations);
		for (auto & future : futures)
			values.push_back(future.get());
		std::this_thread::sleep_for(std::chrono::milliseconds{ 50 });
		std::cout << "\nValues :\n";
		std::copy(values.cbegin(), values.cend(), std::ostream_iterator<double>{std::cout, "\n"});
		for (unsigned i = 1; i < count_of_useless_operations; ++i)
		{
			assert(values[i] == values[i - 1]); // Comparing floating point values ?
		}
		std::cout << "\nAll values are equal !" << std::endl;
	}
	std::cin.get();
}