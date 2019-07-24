#pragma once

#include <ctime>

struct Indirect;

struct DoubleIndirect
{
	Indirect* first, *second;
};

struct Indirect
{
	void* items[20];
};

struct INode
{
	unsigned type;
	unsigned links_count;
	unsigned size_in_bytes;
	unsigned blocks_allocated;
	time_t last_modificated;
	time_t last_accessed;
	void* items[10];
};