#pragma once

#include <iostream>

namespace nostd
{
	template<typename TValueType>
	class SharedPointer;

	template <typename TValueType>
	class SharedControlBlockBase
	{
	public:
		SharedControlBlockBase(TValueType* i_pointer)
			:mp_data{ i_pointer },
			m_shared_count{},
			m_weak_count{} {}

		const TValueType* get() const
		{
			return mp_data;
		}

		TValueType* get()
		{
			return mp_data;
		}

		size_t& weak_ptr_count()
		{
			return m_weak_count;
		}

		size_t& shared_ptr_count()
		{
			return m_shared_count;
		}

		bool is_valid() const
		{
			return mp_data != nullptr;
		}

		virtual void destroyObject() = 0;

		virtual ~SharedControlBlockBase() {}

	protected:

		template <typename ...Args>
		SharedControlBlockBase(Args && ... arguments)
			:mp_data{ new(this + 1)TValueType{std::forward<TValueType>(arguments)...}},
			m_shared_count{},
			m_weak_count{} {}

		size_t m_shared_count;
		size_t m_weak_count;
		TValueType* mp_data;

		template<typename TValueType, typename ...Args>
		friend SharedPointer<TValueType> make_shared(Args &&... args);
	};

	template<typename TValueType>
	struct BlindedBlock : public SharedControlBlockBase<TValueType>
	{
		template <typename ...Args>
		BlindedBlock(Args && ... arguments)
			:SharedControlBlockBase<TValueType>{ arguments... }
		{}

		virtual void destroyObject() override
		{
			std::cout << "Object destroyed" << std::endl;
			this->mp_data->~TValueType();
			this->mp_data = nullptr;
		}

		virtual ~BlindedBlock() override
		{
			std::cout << "Full Destroy" << std::endl;
			::operator delete[](this);
		}

	};

	template<typename TValueType>
	struct RemoteBlock : public SharedControlBlockBase<TValueType>
	{
		RemoteBlock(TValueType* i_pointer)
			:SharedControlBlockBase<TValueType>{ i_pointer }
		{}

		virtual void destroyObject() override
		{
			std::cout << "Object destroyed" << std::endl;
			delete this->mp_data;
			this->mp_data = nullptr;
		}

		virtual ~RemoteBlock() override
		{
			std::cout << "Full Destroy" << std::endl;
			if (this->mp_data) 
				delete this->mp_data;
			::operator delete(this);
		}
	};
}