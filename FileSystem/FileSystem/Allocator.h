#pragma once

namespace FileSystem
{

	class Allocator
	{
	public:
		static constexpr size_t mg_cluster_size = 4096;

		Allocator(void* ip_allocted_memory, size_t available_size);

		void* GetBlock();

		void FreeBlock(void* ip_block_ptr);
	private:
		void* mp_pool_start;
		void* mp_bitset_end;
		void* mp_data_start;


		long long GetFreeBlockIndex();

		size_t ToInteger(void* ip_data, unsigned bytes);
	};

}

