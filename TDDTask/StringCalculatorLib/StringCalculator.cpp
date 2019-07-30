#include "stdafx.h"
#include "StringCalculator.h"

#include <sstream>
#include <functional>

int Add(const std::string &input_string)
{
	auto [splitting_regex, index] = ExtractDelimiters(input_string);
	std::string newString{ input_string.begin() + index, input_string.end() };
	auto string = std::regex_replace(newString, splitting_regex, " ");
	std::vector<int> result;
	{
		std::istringstream is{ string };
		std::copy(std::istream_iterator<int>{is}, 
			std::istream_iterator<int>{},
			std::back_inserter(result));
	}
	int accumulation{};
	for (unsigned i = 0; i < result.size(); ++i)
	{
		if (result[i] < 0)
		{
			std::string bad_items{ "Invalid values :" };
			for (unsigned j = i; j < result.size(); ++j)
			{
				if (result[j] < 0) bad_items.append(std::to_string(result[j]) + ' ');
			}
			throw std::invalid_argument{ bad_items };
		}
		if (result[i] <= 1000) accumulation += result[i];
	}
	return accumulation;
}

std::pair<std::regex, unsigned> ExtractDelimiters(const std::string & string)
{
	if (string.size() < 2 || string.substr(0, 2) != "//")
		return std::make_pair(ConstructRegexFromDelims({ "\n", "," }), 0);
	auto newLine = string.find('\n', 2) + 1;
	std::vector<std::string> delimiters;
	std::regex parser { R"(\[(.*?)\])" };
	std::sregex_iterator it{ string.cbegin() + 2, string.cbegin() + newLine, parser};
	while (it != std::sregex_iterator{})
	{
		delimiters.push_back((*it).str(1));
		++it;
	}
	return std::make_pair(ConstructRegexFromDelims({ delimiters }), newLine);
}

std::regex ConstructRegexFromDelims(const std::vector<std::string> & strings)
{
	static std::string special_characters{ "\^$.|?*+()" };
	std::ostringstream os;
	auto size = strings.size();
	for (unsigned i = 0; i < size; ++i)
	{
		if (i != 0) os << '|';
		os << '(';
		if (std::search(special_characters.cbegin(), special_characters.cend(),
			std::boyer_moore_horspool_searcher{ strings[i].cbegin(),strings[i].cend() })
			!= special_characters.cend())
		{
			os << "\\";
		}
		os << strings[i] << ')';
	}
	return std::regex{ os.str() };
}
