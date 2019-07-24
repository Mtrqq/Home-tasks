#include "Allocator.h"

#include <stdexcept>

namespace FileSystem
{
	Allocator::Allocator(void* ip_allocted_memory, size_t available_size)
		:mp_pool_start{ ip_allocted_memory },
		mp_data_start{ nullptr },
		mp_bitset_end{ nullptr }
	{
		auto blocks_count = available_size / mg_cluster_size;
		auto offset = static_cast<size_t>((double(blocks_count) / 8) + 0.999);
		mp_bitset_end = static_cast<char*>(mp_pool_start) + offset;
		auto clusters_for_indexing = static_cast<size_t>((double(offset) / mg_cluster_size) + 0.999);
		mp_data_start = static_cast<char*>(mp_pool_start) + (mg_cluster_size * clusters_for_indexing);
	}

	void* Allocator::GetBlock()
	{
		auto index = GetFreeBlockIndex();
		if (index == -1) throw std::bad_alloc{};
		return static_cast<char*>(mp_data_start) + (mg_cluster_size * index);
	}

	void Allocator::FreeBlock(void* ip_block_ptr)
	{
		auto distance_to_block = static_cast<char*>(ip_block_ptr) - static_cast<char*>(mp_pool_start);
		if (distance_to_block % mg_cluster_size != 0) throw std::bad_alloc{};
		auto bit_indicator_location = static_cast<char*>(mp_pool_start) + distance_to_block / 8;
		*bit_indicator_location &= ~(1 << (7 - (distance_to_block % 8)));
	}

	long long Allocator::GetFreeBlockIndex()
	{
		unsigned count_of_bytes_analized{ 8 };
		unsigned char* p_walker = static_cast<unsigned char*>(mp_pool_start);
		while ((p_walker < mp_bitset_end) && count_of_bytes_analized)
		{
			if (ToInteger(p_walker, count_of_bytes_analized) != 0)
			{
				count_of_bytes_analized <<= 1;
			}
			else p_walker += count_of_bytes_analized;
		}
		if (count_of_bytes_analized) return -1;

		for (unsigned checked_bit = 0; checked_bit < 8; ++checked_bit)
		{
			if (*p_walker & (1 << (7 - checked_bit)))
			{
				return (p_walker - static_cast<unsigned char*>(mp_pool_start)) + checked_bit;
			}
		}
		return -1;
	}

	size_t Allocator::ToInteger(void* ip_data, unsigned bytes)
	{
		size_t result{};
		unsigned char* p_walker = static_cast<unsigned char*>(ip_data);
		for (unsigned i = 0; i < bytes; ++i)
		{
			result <<= 8;
			result += *p_walker;
			++p_walker;
			//Or should i prefer :
			// result = (result << 8) + *(walker++); ?
		}
		return result;
	}
}