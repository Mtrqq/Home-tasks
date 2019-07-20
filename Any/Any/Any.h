#pragma once

#include "TypeContainer.h"

#include <type_traits>
#include <stdexcept>

class Any
{
public:
	Any()
		:data{ nullptr } {}

	Any(const Any& another)
		:data{ another.data.get()->clone() } {}

	Any(Any&& another) noexcept
		:data{ std::move(another.data) } {}

	template <typename T>
	Any(const T& value)
		: data{ value } {}

	template <typename T>
	Any(T&& value, typename std::enable_if<std::is_move_constructible<T>::value>::type* = nullptr)
		: data{ new T{std::move(value)} } {}

	template <typename T>
	Any& operator=(const T& another)
	{
		data.reset(new SuccessorType<T>(another));
		return *this;
	}

	bool empty() const
	{
		return data == nullptr;
	}

	const std::type_info& type() const
	{
		if (data == nullptr)
			return typeid(void);
		return data.get()->type();
	}

	void clear()
	{
		data.reset();
	}

	template<typename T>
	const T& get() const
	{
		if (data.get()->type() != typeid(T))
			throw std::bad_typeid{};
		return static_cast<SuccessorType<T>*>(data.get())->value();
	}

	template<typename T>
	T& get()
	{
		const Any& self = *this;
		return const_cast<T&>(self.get<T>());
	}

private:
	std::unique_ptr<BaseType> data;
};