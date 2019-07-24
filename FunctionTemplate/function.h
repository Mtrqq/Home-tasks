#pragma once

#include <typeinfo>
#include <utility>

template <typename UnusedType>
class Function;

template <typename ReturnType, typename ...Arguments>
class Function< ReturnType (Arguments...)>
{
public:
	Function(nullptr_t = nullptr) noexcept
		:mp_function{ nullptr }
	{}

	template <typename FunctionType>
	Function(FunctionType function)
		: mp_function{ new ConcreteFunction<FunctionType>(function) }
	{}

	Function(const Function &other)
		:mp_function{ other.mp_function->clone() }
	{}

	Function(Function &&other) noexcept
		: mp_function{ std::move(mp_function) }
	{
		other.mp_function = nullptr;
	}

	~Function()
	{
		delete mp_function;
	}

	template <typename FunctionType>
	Function& operator=(FunctionType function)
	{
		if (mp_function) delete mp_function;
		mp_function = new ConcreteFunction<FunctionType>(function);
		return *this;
	}

	Function& operator=(const Function &other)
	{
		if (mp_function) delete mp_function;
		mp_function = other.mp_function->clone();
		return *this;
	}

	Function& operator=(Function &&function)
	{
		if (mp_function) delete mp_function;
		mp_function = std::move(function.mp_function);
		function.mp_function = nullptr;
		return *this;
	}

	ReturnType operator()(Arguments && ... args)
	{
		return mp_function->invoke(std::forward<Arguments>(args)...);
	}

	operator bool() const noexcept
	{
		return mp_function != nullptr;
	}

	const type_info&  target_type()
	{
		return mp_function ? mp_function->target_type() : typeid(void);
	}

private:
	class FunctionHolder
	{
	public:
		virtual ReturnType invoke(Arguments && ... args) = 0;
		virtual const type_info&  target_type() const = 0;
		virtual FunctionHolder* clone() const = 0;
		virtual ~FunctionHolder() {};
	};

	template <typename FunctionType>
	class ConcreteFunction : public FunctionHolder
	{
	public:

		ConcreteFunction(FunctionType fun)
			:m_function{ fun }
		{}

		virtual ReturnType invoke(Arguments && ... args) override
		{
			return m_function(std::forward<Arguments>(args)...);
		}

		virtual const type_info& target_type() const
		{
			return typeid(m_function);
		}

		virtual FunctionHolder* clone() const override
		{
			return new ConcreteFunction<FunctionType>{ m_function };
		}

		virtual ~ConcreteFunction() override
		{}
	private:

		FunctionType m_function;
	};

	FunctionHolder *mp_function;
};