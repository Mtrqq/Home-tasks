#pragma once


#pragma once

#include <initializer_list>
#include <type_traits>
#include <stdexcept>
#include <cstring>
#include <utility>
#include <cstdlib>

namespace nostd
{

	template<class TValueType>
	class Vector
	{
	public:
		using size_type = size_t;
		using const_iterator = const TValueType*;
		using iterator = TValueType * ;

		Vector() = default;

		Vector(size_type i_size, TValueType i_filler = TValueType())
		{
			assign(i_size, i_filler);
		}

		Vector(std::initializer_list<TValueType> i_init_list)
		{
			assign(std::move(i_init_list));
		}

		Vector(const Vector<TValueType>& i_copied_vector)
		{
			assign(i_copied_vector);
		}

		Vector(Vector<TValueType>&& i_moved_vector) noexcept
		{
			assign(std::move(i_moved_vector));
		}

		~Vector()
		{
			if (mp_container)
				free(mp_container);
		}

		// Sets count of elements equals to i_size, if there was no element
		// in some position constructs default value there
		void resize(size_type i_size);

		// Reserves not less than argument capacity, to avoid reallocations 
		// during insert operations
		void reserve(size_type i_capacity);

		void assign(size_type i_size, TValueType i_filler = TValueType());

		void assign(std::initializer_list<TValueType> i_init_list);

		void assign(const Vector& another);

		void assign(Vector&& another) noexcept;

		const TValueType& at(size_type i_index) const
		{
			if (i_index > m_size) throw std::out_of_range{ "Index out of range" };
			return mp_container[i_index];
		}

		TValueType& at(size_type i_index)
		{
			const Vector<TValueType> &self = *this;
			return const_cast<TValueType&>(self.at(i_index));
		}

		const TValueType& operator[](size_type i_index) const
		{
			return mp_container[i_index];
		}

		TValueType& operator[](size_type i_index)
		{
			const Vector<TValueType> &self = *this;
			return const_cast<TValueType&>(self[i_index]);
		}

		const TValueType& front() const
		{
			return at(0);
		}

		TValueType& front()
		{
			return at(0);
		}

		const TValueType& back() const
		{
			return at(m_size - 1);
		}

		TValueType& back()
		{
			return at(m_size - 1);
		}

		bool empty() const
		{
			return m_size == 0;
		}

		void push_back(const TValueType &value)
		{
			if (m_capacity <= m_size) reserve(m_capacity * mg_resize_factor);
			mp_container[m_size++] = value;
		}

		void push_back(TValueType&& value)
		{
			if (m_capacity <= m_size) reserve(m_capacity * mg_resize_factor);
			mp_container[m_size++] = std::move(value);
		}

		template <class... Args>
		void emplace_back(Args&& ... args)
		{
			if (m_capacity <= m_size) reserve(m_capacity * mg_resize_factor);
			new(mp_container + m_size++) TValueType(std::forward<Args>(args)...);
		}

		void shrink_to_feet(size_type i_size)
		{
			reallocate_memory(i_size);
		}

		void pop_back()
		{
			if (m_size > 0)
			{
				mp_container[--m_size].~TValueType();
			}
		}

		size_type capacity() const
		{
			return m_capacity;
		}

		size_type size() const
		{
			return m_size;
		}

		Vector& operator=(Vector &&another) noexcept
		{
			assign(std::move(another));
			return *this;
		}

		Vector& operator=(const Vector& another)
		{
			assign(another);
			return *this;
		}

		Vector& operator=(std::initializer_list<TValueType> i_init_list)
		{
			assign(std::move(i_init_list));
			return *this;
		}


		const_iterator cbegin() const
		{
			return mp_container;
		}

		const_iterator cend() const
		{
			return mp_container + m_size;
		}

		iterator begin() const
		{
			const Vector<TValueType> &self = *this;
			return const_cast<iterator>(cbegin());
		}

		iterator end() const
		{
			const Vector<TValueType> &self = *this;
			return const_cast<iterator>(cend());
		}

	private:

		void reallocate_memory(size_type i_size);

		TValueType* mp_container = nullptr;
		size_t m_capacity = 0, m_size = 0;

		constexpr static size_type mg_resize_factor = 2;
	};


	template<class TValueType>
	void Vector<TValueType>::resize(size_type i_size)
	{
		if (i_size > m_size)
		{
			if (m_capacity < i_size)
				reserve(i_size);
			for (auto i = m_size; i < i_size; ++i)
			{
				mp_container[i] = TValueType{};
			}
		}
		m_size = i_size;
	}

	template<class TValueType>
	void Vector<TValueType>::reserve(size_type i_capacity)
	{
		if (i_capacity == 0) i_capacity = 1;
		if (m_capacity < i_capacity)
		{
			reallocate_memory(i_capacity);
			m_capacity = i_capacity;
		}
	}

	template<class TValueType>
	void Vector<TValueType>::assign(size_type i_size, TValueType i_filler)
	{
		resize(i_size);
		for (int i = 0; i < i_size; ++i)
		{
			mp_container[i] = i_filler;
		}
	}

	template<class TValueType>
	void Vector<TValueType>::assign(std::initializer_list<TValueType> i_init_list)
	{
		reserve(i_init_list.size());
		size_type entry_counter{};
		for (auto& element : i_init_list)
		{
			mp_container[entry_counter++] = std::move_if_noexcept(element);
		}
		m_size = i_init_list.size();
	}

	template<class TValueType>
	void Vector<TValueType>::assign(const Vector<TValueType>& i_copied_vector)
	{
		if (this != &i_copied_vector)
		{
			m_size = i_copied_vector.m_size;
			reserve(i_copied_vector.m_capacity);
			for (int i = 0; i < m_size; ++i)
			{
				mp_container[i] = i_copied_vector[i];
			}
		}
	}

	// somehow avoid recursion
	template<class TValueType>
	void Vector<TValueType>::assign(Vector<TValueType>&& i_moved_vector) noexcept
	{
		if (this != &i_moved_vector)
		{
			mp_container = std::move(i_moved_vector.mp_container);
			m_size = i_moved_vector.m_size;
			m_capacity = i_moved_vector.m_capacity;
			i_moved_vector.mp_container = nullptr;
			i_moved_vector.m_capacity = 0;
			i_moved_vector.m_size = 0;
		}
	}

	template<class TValueType>
	inline void Vector<TValueType>::reallocate_memory(size_type i_capacity)
	{
		auto buffer = mp_container;
		mp_container = reinterpret_cast<TValueType*> (malloc(i_capacity * sizeof(TValueType)));
		if (buffer)
		{
			if /*constexpr*/ (std::is_pod<TValueType>::value)
			{
				std::memmove(mp_container, buffer, m_size * sizeof(TValueType));
			}
			else
			{
				for (unsigned i = 0; i < m_size; ++i)
				{
					mp_container[i] = std::move_if_noexcept(buffer[i]);
				}
			}
			free(buffer);
		}
	}

}