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
		SharedControlBlockBase()
			:m_shared_count{ 1 },
			m_weak_count{} {}

		virtual const TValueType* get() const = 0;

		virtual TValueType* get() = 0;

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
			return get() != nullptr;
		}

		virtual void destroyObject() = 0;

		virtual ~SharedControlBlockBase() {}

	protected:
		size_t m_shared_count;
		size_t m_weak_count;
	};

	template<typename TValueType>
	class BlindedBlock : public SharedControlBlockBase<TValueType>
	{
	public:
		template <typename ...Args>
		BlindedBlock(Args && ... arguments)
			:data{std::forward<Args>(arguments)...}
		{}

		virtual void destroyObject() override
		{
			std::cout << "Object destroyed" << std::endl;
			data.~TValueType();
		}

		virtual ~BlindedBlock() override
		{
			std::cout << "Full Destroy" << std::endl;
		}

		virtual const TValueType* get() const
		{
			return &data;
		}

		virtual TValueType* get()
		{
			return &data;
		}

	private:
		TValueType data;
	};

	template<typename TValueType>
	class RemoteBlock : public SharedControlBlockBase<TValueType>
	{
	public:
		RemoteBlock(TValueType* i_pointer)
			:data{i_pointer}
		{}

		virtual void destroyObject() override
		{
			std::cout << "Object destroyed" << std::endl;
			delete data;
			data = nullptr;
		}

		virtual ~RemoteBlock() override
		{
			std::cout << "Full Destroy" << std::endl;
			if (data) delete data;
		}

		virtual const TValueType* get() const
		{
			return data;
		}

		virtual TValueType* get()
		{
			return data;
		}

	private:
		TValueType* data;
	};
}