#pragma once

namespace nostd
{

	template<typename TValueType>
	class SharedPointer;

	template<typename TValueType>
	class SharedControlBlockBase;

	template<typename TValueType>
	class WeakPointer
	{
	public:
		WeakPointer(): mp_control_block{nullptr}{}

		WeakPointer(const SharedPointer<TValueType> &shared)
			:mp_control_block{shared.mp_control_block}
		{
			++mp_control_block->weak_ptr_count();
		}

		WeakPointer(const WeakPointer<TValueType>& weak)
			:mp_control_block{ weak.mp_control_block }
		{
			++mp_control_block->weak_ptr_count();
		}

		WeakPointer& operator=(const SharedPointer<TValueType>& shared)
		{
			--mp_control_block->weak_ptr_count();
			deleteIfRealeased();
			mp_control_block = shared.mp_control_block;
			++mp_control_block->weak_ptr_count();
		}

		~WeakPointer()
		{
			if (mp_control_block)
			{
				--mp_control_block->weak_ptr_count();
				deleteIfRealeased();
			}
		}

		SharedPointer<TValueType> lock()
		{
			return { mp_control_block };
		}

		bool is_valid()
		{
			return mp_control_block->is_valid();
		}

		void deleteIfRealeased()
		{
			if (mp_control_block->weak_ptr_count() == 0 &&
				mp_control_block->shared_ptr_count() == 0)
			{
				mp_control_block->~SharedControlBlockBase();
			}
		}

	private:
		SharedControlBlockBase<TValueType>* mp_control_block;
	};

}
