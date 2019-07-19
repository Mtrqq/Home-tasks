#pragma once

template<typename T, typename U, typename ...Args>
struct index_of_type
{
	static constexpr int value = index_of_type<T, Args...>::value + 1;
};

template<typename T, typename ...Args>
struct index_of_type<T, T, Args...>
{
	static constexpr int value = 0;
};

template<typename T, typename U>
struct index_of_type<T, U>
{
	static constexpr int value = 1;
};

template<typename T>
struct index_of_type<T, T>
{
	static constexpr int value = 0;
};


template<typename T, typename ...Args>
struct size_extractor
{
	static constexpr size_t max_size()
	{
		constexpr size_t temporary = size_extractor<Args...>::max_size();
		return sizeof(T) > temporary ? sizeof(T) : temporary;
	}
};

template <typename T>
struct size_extractor<T>
{
	static constexpr size_t max_size()
	{
		return sizeof(T);
	}
};

template <size_t N, typename T, typename ...Args>
struct NthType
{
	typedef typename NthType<N - 1, Args...>::type type;
};

template <typename T, typename ...Args>
struct NthType<0, T, Args...>
{
	typedef T type;
};

template <typename T>
struct NthType<0, T>
{
	typedef T type;
};