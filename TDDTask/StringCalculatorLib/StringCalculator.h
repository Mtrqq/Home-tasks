#pragma once

#include <regex>
#include <string>
#include <vector>

std::pair<std::regex, unsigned> ExtractDelimiters(const std::string & string);

std::regex ConstructRegexFromDelims(const std::vector<std::string> & strings);

int Add(const std::string &str);