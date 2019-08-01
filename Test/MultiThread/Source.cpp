#include <vector>
#include <chrono>
#include <iostream>
#include <random>
#include <algorithm>
#include <iomanip>
#include <thread>

void ParallelAccumulate(std::vector<double> &vec, double add, int threads_count)
{
	std::vector<std::thread> threads;
	threads.reserve(threads_count);
	auto data_amount = vec.size();
	for (int i = 0; i < threads_count; ++i)
	{
		auto function = [&] {
			for (int j = i; j < data_amount; j += threads_count)
			{
				vec[j] += add;
			}
		};
		threads.emplace_back(function);
	}
	for (auto &thread : threads)
		thread.join();
}

using namespace std::chrono;

double ParallelAlgorithmTester(std::vector<double> &vec, double add, int count_of_threads)
{
	if (count_of_threads == 0) throw std::invalid_argument{ "zero amount of threads" };
	auto start = steady_clock::now();
	ParallelAccumulate(vec, add, count_of_threads);
	auto end_time = duration<double>(steady_clock::now() - start).count();
	std::cout << count_of_threads << " thread(s) time:" << end_time << std::endl;
	return end_time;
}

int main()
{
	/*std::ios::sync_with_stdio(false);
	std::mt19937 generator(std::random_device{}());
	std::uniform_real_distribution<> u_rd{ 0 ,10 };
	std::vector<double> values(100'000'000);
	std::generate(values.begin(), values.end(), [&] { return u_rd(generator); });
	auto one_time = ParallelAlgorithmTester(values, 500, 1);
	ParallelAlgorithmTester(values, 500, 2);
	auto max_time = ParallelAlgorithmTester(values, 500, 4);
	ParallelAlgorithmTester(values, 500, 16);
	std::cout << "\nSpeedUp :" << one_time / max_time;*/
	constexpr auto N = 10000;
	std::vector<double> vec;
	vec.reserve(N * N);
	vec.resize(N * N);
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			vec[i * N + j] = 2;
		}
	}
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			vec[i + j * N] = 3;
		}
	}
	std::cin.get();
}