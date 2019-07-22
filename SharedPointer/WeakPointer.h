#pragma once

template<typename TValueType>
class SharedPointer;

template<typename TValueType>
class SharedControlBlock;

namespace nostd
{

	template<typename TValueType>
	class WeakPointer
	{
	public:
		WeakPointer(const SharedPointer<TValueType> &shared)
			:mp_control_block{shared.mp_control_block}
		{
			++mp_control_block->weak_ptr_count();
		}

		~WeakPointer()
		{
			if (mp_control_block->weak_ptr_count() == 0 &&
				--mp_control_block->shared_ptr_count() == 0)
				 mp_control_block->~SharedControlBlockBase()
		}

		SharedPointer<TValueType> lock()
		{
			return { mp_control_block };
		}

		bool is_valid()
		{
			return mp_control_block->is_valid();
		}

	private:
		SharedControlBlock* mp_control_block;
	};

}
