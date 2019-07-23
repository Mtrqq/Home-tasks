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
		{
			changeControlBlock(shared);
		}

		WeakPointer(const WeakPointer<TValueType>& weak)
		{
			changeControlBlock(weak);
		}

		WeakPointer& operator=(const SharedPointer<TValueType>& shared)
		{
			if (mp_control_block)
			{
				--mp_control_block->weak_ptr_count();
				deleteIfRealeased();
			}
			changeControlBlock(shared);
			return *this;
		}

		WeakPointer& operator=(const WeakPointer<TValueType>& weak)
		{
			if (mp_control_block)
			{
				--mp_control_block->weak_ptr_count();
				deleteIfRealeased();
			}
			changeControlBlock(weak);
			return *this;
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
			return SharedPointer<TValueType>{ mp_control_block };
		}

		bool is_valid()
		{
			return mp_control_block != nullptr && mp_control_block->is_valid();
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

		template <typename BlockOwner>
		void changeControlBlock(const BlockOwner& owner)
		{
			if (mp_control_block)
			{
				--mp_control_block->weak_ptr_count();
				deleteIfRealeased();
			}
			if (owner.mp_control_block)
			{
				mp_control_block = owner.mp_control_block;
				++mp_control_block->weak_ptr_count();
			}
		}

		template <typename TValueType>
		friend class SharedPointer;
	};

}
