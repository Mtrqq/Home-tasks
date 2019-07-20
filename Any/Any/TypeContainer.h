#pragma once

#include <typeinfo>
#include <memory>

class BaseType
{
public:
	virtual const std::type_info& type() const = 0;
	virtual std::unique_ptr<BaseType> clone() const = 0;
	virtual ~BaseType() = default;
};

template <typename T>
class SuccessorType : public BaseType
{
public:
	SuccessorType(const SuccessorType& another)
		:value{another.m_value}{}

	SuccessorType(SuccessorType && another)
		:value{std::move(another.m_value)}{}

	SuccessorType(const T& another):m_value{another}
	{
	}

	virtual std::unique_ptr<BaseType> clone() const override
	{
		return std::unique_ptr<BaseType>(new SuccessorType<T>{ m_value });
	}

	virtual const std::type_info& type() const override
	{
		return typeid(T);
	}

	T& value()
	{
		return m_value;
	}
private:
	T m_value;
};