#pragma once

#include <memory>
#include <functional>
#include <typeinfo>
#include <unordered_map>
#include <stdexcept>
#include <typeindex>

class TypeIndexHasher
{
public:
	size_t operator()(const std::type_index& type) const
	{
		return type.hash_code();
	}
};

class InterfaceFactory
{
	template <typename TInterface>
	using CreatorFunction = std::function<std::unique_ptr<TInterface>()>;

	class FunctionHolder
	{
	public:
		virtual ~FunctionHolder() = 0;
	};

	template <typename TFunctionCreator>
	class ConcreteFunctionHolder : public FunctionHolder
	{
		TFunctionCreator held_object;
	public:
		ConcreteFunctionHolder(const TFunctionCreator& i_function)
			:held_object{ i_function }
		{}
		TFunctionCreator& Get()
		{
			return held_object;
		}
		virtual ~ConcreteFunctionHolder() override
		{}
	};
public:
	template <typename TInterface>
	class ScopedConnection
	{
	public:
		ScopedConnection() = default;
		~ScopedConnection()
		{
			InterfaceFactory::Instance().Unregister<TInterface>();
		}
		ScopedConnection(const ScopedConnection& i_other) = delete;
	};

	template<class TInterface>
	bool IsRegistered() const;

	template<class TInterface, class TImplementation>
	void Register();

	template<class TInterface>
	void Register(const CreatorFunction<TInterface> &i_create_proc);

	template<class TInterface, class TImplementation>
	ScopedConnection<TInterface> ScopedRegister();

	template<class TInterface>
	ScopedConnection<TInterface> ScopedRegister(const CreatorFunction<TInterface>& i_create_proc);

	template<class TInterface>
	void Unregister();

	template<class TInterface>
	std::unique_ptr<TInterface> Create() const;

	template<class TInterface>
	TInterface* CreateRaw() const;

	static InterfaceFactory& Instance()
	{
		static InterfaceFactory instance;
		return instance;
	}

	InterfaceFactory& operator=(const InterfaceFactory& i_other) = delete;
	InterfaceFactory(const InterfaceFactory& i_other) = delete;

private:
	InterfaceFactory() = default;
	std::unordered_map<std::type_index, FunctionHolder*,TypeIndexHasher> m_creator_functions;
};

template<class TInterface>
bool InterfaceFactory::IsRegistered() const
{
	return m_creator_functions.find(typeid(TInterface)) != m_creator_functions.end();
}

template<class TInterface, class TImplementation>
void InterfaceFactory::Register()
{
	auto creator_function_wrapper = [] {return std::unique_ptr<TInterface>{new TImplementation}; };
	m_creator_functions[typeid(TInterface)] = new ConcreteFunctionHolder<CreatorFunction<TInterface>>(creator_function_wrapper);
}

template<class TInterface>
void InterfaceFactory::Register(const CreatorFunction<TInterface>& i_create_proc)
{
	m_creator_functions[typeid(TInterface)] = new ConcreteFunctionHolder<CreatorFunction<TInterface>>(i_create_proc);
}

template<class TInterface, class TImplementation>
InterfaceFactory::ScopedConnection<TInterface> InterfaceFactory::ScopedRegister()
{
	Register<TInterface, TImplementation>();
	return ScopedConnection<TInterface>{};
}

template<class TInterface>
InterfaceFactory::ScopedConnection<TInterface> 
InterfaceFactory::ScopedRegister(const CreatorFunction<TInterface>& i_create_proc)
{
	Register<TInterface>(i_create_proc);
	return ScopedConnection<TInterface>{};
}

template<class TInterface>
inline void InterfaceFactory::Unregister()
{
	m_creator_functions.erase(typeid(TInterface));
}

template<class TInterface>
std::unique_ptr<TInterface> InterfaceFactory::Create() const
{
	auto function = m_creator_functions.find(typeid(TInterface));
	if (function == m_creator_functions.end())
		throw std::runtime_error{"unregistered object creation attemp"};
	auto function_holder = dynamic_cast<ConcreteFunctionHolder<CreatorFunction<TInterface>>*>(function->second);
	return (function_holder->Get())();
}

template<class TInterface>
TInterface* InterfaceFactory::CreateRaw() const
{
	return Create<TInterface>().release();
}
