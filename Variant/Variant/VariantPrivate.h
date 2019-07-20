#pragma once

#include "VariantHelperFunctions.h"

#include <stdexcept>

// Should I replace bad_typeid for custom exception in get<T> method ?
// enable_if doesn't works with constexpr function get_type_index, why ?

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

	//add move constructor ?
	// is it correct to use memmove on c style array ?


	template <typename T>
	typename std::enable_if <HelperFunctions::index_of_type<T, Args...>::value < sizeof...(Args), T&>::type get()
	{
		if (type_index != -1 && get_type_index<T>() == type_index)
		{
			return *reinterpret_cast<T*>(data);
		}
		throw std::bad_typeid{}; // change for some custom exception ?
	}

	template <size_t N>
	typename std::enable_if < N < sizeof...(Args), typename HelperFunctions::NthType<N, Args...>::type&>::type get()
	{
		if (type_index == N)
		{
			return *reinterpret_cast<typename HelperFunctions::NthType<N, Args...>::type*>(data);
		}
		throw std::invalid_argument{ "invalid index" };
	}

	template<typename T>
	typename std::enable_if < HelperFunctions::index_of_type<T, Args...>::value < sizeof...(Args), void>::type assign(const T& value)
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
	typename std::enable_if < HelperFunctions::index_of_type<T, Args...>::value < sizeof...(Args), void>::type assign(T&& value)
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
	char data[HelperFunctions::size_extractor<Args...>::max_size()];
	int type_index = -1;

	template<typename T>
	constexpr int get_type_index() const
	{
		constexpr auto index = HelperFunctions::index_of_type<T, Args...>::value;
		if (index >= sizeof...(Args)) return -1;
		return index;
	}

};
