#pragma once

#include "VariantPrivate.h"

template <typename ...Args>
class Variant
{
public:
	Variant() = default;
	
	template <typename T>
	Variant(const T& i_value)
	{
		mp_impl.assign(i_value);
	}

	template <typename T>
	Variant (T&& i_value)
	{
		mp_impl.assign(std::move(i_value));
	}

	Variant(const Variant& another)
	{
		mp_impl = another.mp_impl;
	}

	template <typename T>
	Variant& operator=(const T& i_value)
	{
		mp_impl.assign(i_value);
		return *this;
	}

	template <typename T>
	Variant& operator=(T&& i_value)
	{
		mp_impl.assign(std::move(i_value));
		return *this;
	}

	template <typename T>
	T get()
	{
		return mp_impl.get<T>(); // Avoid second copy by std::move() ?
	}

	template <size_t N>
	typename NthType<N, Args...>::type get()
	{
		return mp_impl.get<N>();
	}

	size_t index() const
	{
		return mp_impl.index();
	}
private:
	VariantPrivate<Args...> mp_impl;
};