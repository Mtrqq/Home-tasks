#pragma once

#include "Function.h"

namespace nostd
{

	class ScopedCaller
	{
	public:
		ScopedCaller() = default;

		template<typename Function>
		ScopedCaller(Function && f)
			: m_function(std::forward<Function>(f))
		{}

		ScopedCaller(ScopedCaller&& other)
		{
			m_function = std::move(other.m_function);
		}

		ScopedCaller(const ScopedCaller &another) = delete;

		~ScopedCaller() noexcept
		{
			InvokeFunction();
		}

		template<typename Function>
		void Reset(Function && f)
		{
			//InvokeFunction();			// Should i call this ?
			m_function = std::forward<Function>(f);
		}

		void Release() noexcept			// return something ?
		{
			m_function.nulify();
		}

	private:
		nostd::Function<void()> m_function;

		void InvokeFunction() noexcept
		{
			if (m_function)
			{
				m_function();
				m_function.nulify();
			}
		}
	};

}