#pragma once

namespace nostd
{
	/*template<typename TValueType, typename ...Args>
	SharedPointer<TValueType> make_shared(Args &&... args)
	{
		char* p_data_pointer = new char[sizeof(BlindedBlock<TValueType>) + sizeof(TValueType)];
		new(p_data_pointer) BlindedBlock<TValueType>{args...};
		return SharedPointer<TValueType>{static_cast<BlindedBlock<TValueType>>(p_data_pointer)};
	}*/

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

	protected:

		template <typename ...Args>
		SharedControlBlockBase(Args && ... arguments)
			:mp_data{ this + 1 },
			m_shared_count{},
			m_weak_count{} {}

	private:
		size_t m_shared_count;
		size_t m_weak_count;
		TValueType* mp_data;

	/*	template<typename TValueType, typename ...Args>
		friend SharedPointer<TValueType> make_shared(Args &&... args);*/
	};

	template<typename TValueType>
	struct BlindedBlock : public SharedControlBlockBase<TValueType>
	{
		template <typename ...Args>
		BlindedBlock(Args && ... arguments)
			:SharedControlBlockBase{ arguments... }
		{}

		virtual void destroyObject() override
		{
			this->mp_data->~TValueType();
			this->mp_data = nullptr;
		}

		virtual ~BlindedBlock() override
		{
			delete[] this;
		}
	};

	template<typename TValueType>
	struct RemoteBlock : public SharedControlBlockBase<TValueType>
	{
		RemoteBlock(TValueType* i_pointer)
			:SharedControlBlockBase{ i_pointer }
		{}

		virtual void destroyObject() override
		{
			delete this->mp_data;
			this->mp_data = nullptr;
		}

		virtual ~RemoteBlock() override
		{
			delete this->mp_data;
			delete this;
		}
	};
}