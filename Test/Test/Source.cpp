#include <iostream>
#include <thread>
#include <chrono>
#include <numeric>
#include <vector>
#include <random>
#include <iterator>
#include <iomanip>

template <typename It, typename T,typename Proccessor>
void SummatorHelper(It begin, It end, Proccessor i_func, T& o_result)
{
	T value{};
	while (begin != end)
	{
		value += i_func(*begin);
		++begin;
	}
	o_result = value;
}

template <typename It, typename T, typename Proccessor>
T ParallelAccumulate(It begin, It end, T accumulator,
		Proccessor i_func, int threads_count)
{
	std::vector<std::thread> threads;
	constexpr auto memory_align = 8;
	std::vector<T> values(threads_count * memory_align);
	threads.reserve(threads_count);
	unsigned long long count_of_elements = end - begin;
	unsigned long long data_for_thread = count_of_elements / threads_count;
	It walker = begin;
	for (int i = 0; i < threads_count - 1; ++i)
	{
		auto function = [&] {
			SummatorHelper(walker, (walker + data_for_thread),
								i_func, values[i * memory_align]); 
		};
		threads.emplace_back(function);
		walker += data_for_thread;
	}
	auto function = [&] { SummatorHelper(walker, end, i_func, values.back()); };
	threads.emplace_back(function);
	for (auto &thread : threads)
		thread.join();
	return std::accumulate(values.cbegin(), values.cend(), accumulator);
}

template <typename T, typename Proccessor>
double ParallelAlgorithmTester(const std::vector<T> &vec,Proccessor function, int count_of_threads)
{
	if (count_of_threads == 0) throw std::invalid_argument{ "zero amount of threads" };
	using namespace std::chrono;
	auto start = steady_clock::now();
	auto value = ParallelAccumulate(vec.cbegin(), vec.cend(), 0.0, function, count_of_threads);
	auto end_time = duration<double>(steady_clock::now() - start).count();
	std::cout << count_of_threads << " thread(s) : "
		<< std::fixed << std::setprecision(8) << std::setw(15) << value
		<< "\tTime:" << end_time << std::endl;
	return end_time;
}



int main()
{
	std::ios::sync_with_stdio(false);
	std::mt19937 generator(std::random_device{}());
	std::uniform_real_distribution<> u_rd{ 0, 10000 };
	std::vector<double> values(10'000'000);
	std::generate(values.begin(), values.end(), [&] { return u_rd(generator); });
	auto proccessor = [](double val) { return std::log(std::sqrt(val)); };
	auto one_time = ParallelAlgorithmTester(values, proccessor, 1);
	ParallelAlgorithmTester(values, proccessor, 2);
	auto max_time = ParallelAlgorithmTester(values, proccessor, 4);
	auto real_accumulation = std::accumulate(values.cbegin(), values.cend(), 0.0,
	[&proccessor](double sum, double curr)
	{
		return sum + proccessor(curr);
	});
	std::cout << "\nReal accumulation : " << real_accumulation
			<< std::endl << "Speed up :" << one_time / max_time;
	std::cin.get();
}