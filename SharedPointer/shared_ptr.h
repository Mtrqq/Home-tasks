#pragma once

namespace nostd
{
	template <typename TValueType>
	struct SharedControlBlock
	{
		size_t shared_count;
		size_t weak_count;
		TValueType* pointer;

		void decrease_shared_count();

		SharedControlBlock(TValueType* i_pointer);

		template <typename ...Args>
		SharedControlBlock
	};

	template<typename TValueType>
	class shared_ptr
	{
	public:
		shared_ptr() = default;

		shared_ptr(TValueType* pointer);
		
		shared_ptr(const shared_ptr &another);

		size_t uses_count() const
		{
			return mp_control_block ? mp_control_block->shared_count : 0;
		}

		const TValueType* get() const
		{
			return data;
		}

		TValueType* get()
		{
			return data;
		}

		operator bool() const
		{
			return data != nullptr;
		}

		void reset(TValueType* i_pointer)
		{
			if (mp_control_block)
			{
				--mp_control_block->shared_count;
			}
			data = pointer;
			counter = new SharedControlBlock{ 0,0 };
		}

		~shared_ptr();
	private:
		SharedControlBlock *mp_control_block = nullptr;
	};

	template<typename TValueType>
	shared_ptr<TValueType>::shared_ptr(TValueType * pointer)
	{
		reset(pointer);
	}

	template<typename TValueType>
	shared_ptr<TValueType>::shared_ptr(const shared_ptr & another)
	{
		data = another.data;
		counter = another.counter;
		++(*counter);
	}

	template<typename TValueType>
	shared_ptr<TValueType>::~shared_ptr()
	{
		if (data && mp_control_block)
		{
			if (mp_control_block->shared_count <= 0)
			{
				delete data;
				delete mp_control_block;
			}
		}
	}

}
