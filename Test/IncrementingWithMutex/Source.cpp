#include "ParallelLogger.h"
#include "CustomStruct.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <string>

void LoggerTest(const std::string &file_name,const std::string &appended_text)
{
	ParallelLogger logger_array[5];
	std::vector<std::thread> threads;
	for (int i = 0; i < 5; ++i)
	{
		logger_array[i].SetFileName(file_name);
		threads.emplace_back(&ParallelLogger::Log, std::ref(logger_array[i]), appended_text + std::to_string(i));
	}
	for (int i = 0; i < 5; ++i)
	{
		threads[i].join();
	}
}

void AtomicStructTest()
{
	std::vector<std::thread> threads;
	auto increment_struct_1_million_times = 
		[](std::atomic<CustomStruct> &variable)
	{
		for (unsigned i = 0; i < 1000000; ++i)
		{
			CustomStruct temporary = variable;
			while (!variable.compare_exchange_weak(temporary, temporary + 1));
		}
	};
	std::atomic<CustomStruct> variable{ {} };
	for (unsigned i = 0; i < 10; ++i)
	{
		threads.emplace_back(increment_struct_1_million_times, std::ref(variable));
	}
	for (auto &thread : threads)
		thread.join();
	CustomStruct dest = variable;
	std::cout << "CustomStruct values : \nFloat : " << dest.GetFloatValue()
			  << "\nDouble : " << dest.GetDoubleValue() << std::endl;
}


int main()
{
	LoggerTest("log.txt","Hello world !");
	//AtomicStructTest();
	std::cout << "Done!";
	std::cin.get();
}