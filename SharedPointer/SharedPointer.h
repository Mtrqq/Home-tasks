#pragma once

#include "SharedControlBlock.h"
#include "WeakPointer.h"

#include <utility>
#include <new>

namespace nostd
{
	template<typename TValueType>
	class SharedPointer;

	template<typename TValueType, typename ...Args>
	SharedPointer<TValueType> make_shared(Args && ... args)
	{
		SharedControlBlockBase<TValueType>* p_data_pointer =
			new BlindedBlock<TValueType>{ std::forward<Args>(args)... };
		return SharedPointer<TValueType>{p_data_pointer};
	}

	template<typename TValueType>
	class SharedPointer
	{
	public:
		SharedPointer() = default;

		SharedPointer(TValueType* pointer);

		SharedPointer(const SharedPointer &another);

		SharedPointer(SharedPointer&& another);

		SharedPointer(const WeakPointer<TValueType>&);

		SharedPointer& operator=(const SharedPointer& another);

		SharedPointer& operator=(SharedPointer&& another);

		size_t uses_count() const
		{
			return mp_control_block ? mp_control_block->shared_ptr_count() : 0;
		}

		const TValueType* get() const
		{
			return mp_control_block->get();
		}

		TValueType* get()
		{
			return mp_control_block->get();
		}

		TValueType& operator*()
		{
			return *get();
		}

		const TValueType& operator*() const
		{
			return *get();
		}

		operator bool() const
		{
			return is_valid();
		}

		bool is_valid() const
		{
			return mp_control_block != nullptr && mp_control_block->is_valid();
		}

		void reset(TValueType* i_pointer = nullptr);

		void swap(SharedPointer& another)
		{
			auto* tmp = std::move(another.mp_control_block);
			another.mp_control_block = std::move(mp_control_block);
			mp_control_block = std::move(tmp);
		}

		void deleteIfRealeased();

		~SharedPointer();
	private:
		SharedControlBlockBase<TValueType> *mp_control_block;

		SharedPointer(SharedControlBlockBase<TValueType> *block)
			:mp_control_block{ block } {}

		template <typename BlockOwner>
		void changeControlBlock(const BlockOwner& owner)
		{
			if (owner.mp_control_block)
			{
				mp_control_block = owner.mp_control_block;
				++mp_control_block->shared_ptr_count();
			}
		}

		template <typename TValueType>
		friend class WeakPointer;

		template<typename TValueType, typename ...Args>
		friend SharedPointer<TValueType> make_shared(Args &&... args);
	};

	template<typename TValueType>
	SharedPointer<TValueType>::SharedPointer(TValueType * pointer)
	{
		reset(pointer);
	}

	template<typename TValueType>
	SharedPointer<TValueType>::SharedPointer(const SharedPointer & another)
		:mp_control_block{nullptr}
	{
		changeControlBlock(another);
	}

	template<typename TValueType>
	SharedPointer<TValueType>::SharedPointer(SharedPointer&& another)
		:mp_control_block{ nullptr }
	{
		operator=(another);
	}

	template<typename TValueType>
	SharedPointer<TValueType>::SharedPointer(const WeakPointer<TValueType>&another)
		:mp_control_block{ nullptr }
	{
		mp_control_block = another.mp_control_block;
		++mp_control_block->shared_ptr_count();
	}

	template<typename TValueType>
	SharedPointer<TValueType>& SharedPointer<TValueType>::operator=(const SharedPointer<TValueType>& another)
	{
		if (mp_control_block)
		{
			--mp_control_block->shared_ptr_count();
			deleteIfRealeased();
		}
		if (this != &another)
		{
			changeControlBlock(another);
		}
		return *this;
	}

	template<typename TValueType>
	SharedPointer<TValueType>& SharedPointer<TValueType>::operator=(SharedPointer<TValueType>&& another)
	{
		if (mp_control_block)
		{
			--mp_control_block->shared_ptr_count();
			deleteIfRealeased();
		}
		if (this != &another)
		{
			mp_control_block = std::move(another.mp_control_block);
			another.mp_control_block = nullptr;
		}
		return *this;
	}

	template<typename TValueType>
	void SharedPointer<TValueType>::reset(TValueType * i_pointer)
	{
		if (mp_control_block)
		{
			--mp_control_block->shared_ptr_count();
			deleteIfRealeased();
		}
		if (i_pointer)
		{
			mp_control_block = new RemoteBlock<TValueType>{ i_pointer };
		}
		else mp_control_block = nullptr;
	}

	template<typename TValueType>
	void SharedPointer<TValueType>::deleteIfRealeased()
	{
		if (mp_control_block->shared_ptr_count() == 0)
		{
			mp_control_block->destroyObject();
			if (mp_control_block->weak_ptr_count() == 0)
			{
				delete mp_control_block;
			}
		}
	}

	template<typename TValueType>
	SharedPointer<TValueType>::~SharedPointer()
	{
		if (mp_control_block && mp_control_block->is_valid())
		{
			--mp_control_block->shared_ptr_count();
			deleteIfRealeased();
		}
	}

}
