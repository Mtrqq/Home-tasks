#pragma once

#include "VariantHelperFunctions.h"

#include <stdexcept>

// Custom exception ?
// Move C style array ? 
// Errors with const get function ?

template <typename ...Args>
class VariantPrivate
{
public:
	VariantPrivate() = default;

	VariantPrivate(const VariantPrivate<Args...>& another)
	{
		data = another.data;
		type_index = another.type_index;
	}

	template <typename T>
	T get()
	{
		if (type_index != -1 && get_type_index<T>() == type_index)
		{
			return *reinterpret_cast<T*>(data);
		}
		throw std::bad_typeid{}; // change for some custom exception ?
	}

	template <size_t N>
	typename NthType<N, Args...>::type get()
	{
		if (N < sizeof...(Args))
		{
			return *reinterpret_cast<typename NthType<N, Args...>::type*>(data);
		}
		throw std::invalid_argument{ "invalid index" };
	}

	template<typename T>
	void assign(const T& value)
	{
		auto index = get_type_index<T>();
		if (index != -1)
		{
			*reinterpret_cast<T*>(data) = T{ value };
			type_index = index;
		}
		else throw std::bad_typeid{};
	}

	template<typename T>
	void assign(T&& value)
	{
		auto index = get_type_index<T>();
		if (index != -1)
		{
			*reinterpret_cast<T*>(data) = T{ std::move(value) };
			type_index = index;
		}
		else throw std::bad_typeid{};
	}

	const int index() const
	{
		return type_index;
	}
private:
	char data[size_extractor<Args...>::max_size()];
	int type_index = -1;

	template<typename T>
	int get_type_index() const
	{
		constexpr auto index = index_of_type<T, Args...>::value;
		if (index >= sizeof...(Args)) return -1;
		return index;
	}
};