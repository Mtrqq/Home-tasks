#pragma once

#include <unordered_map>
#include <mutex>

class ParallelLogger
{
	using lock_g = std::lock_guard<std::mutex>;
public:
	ParallelLogger() = default;
	ParallelLogger(const std::string &fileName);

	void SetFileName(const std::string &fileName);

	void Log(const std::string &label);
private:
	std::string m_file_name;

	static bool FileExists(const std::string &f_name);

	static std::unordered_map<std::string, std::mutex> g_mutex_map;
};

