#pragma once

template <typename TValueType, size_t X, size_t Y>
class Matrix;

template <size_t X, size_t Y, size_t N, size_t K,
	typename MatrixIterator1, typename MatrixIterator2,
	typename TValueType, typename AnotherType> struct NewMatrixType
{
	using type = typename std::enable_if < traits<MatrixIterator1>::mg_token_size == traits<MatrixIterator2>::mg_token_size,
		typename Matrix<typename std::common_type<TValueType, AnotherType>::type, traits<MatrixIterator1>::mg_iteration_count,
		traits<MatrixIterator2>::mg_iteration_count>>::type;
};

template <typename TValueType, size_t X, size_t Y,
	typename AnotherType, size_t N, size_t K,
	template <class, size_t, size_t, typename> class Iterator1,
	template <class, size_t, size_t, typename> class Iterator2,
	typename ValArrayProccessor>
	typename NewMatrixType<X, Y, N, K, Iterator1<TValueType, X, Y, typename std::common_type<TValueType, AnotherType>::type>,
	Iterator2<AnotherType, N, K, typename std::common_type<TValueType, AnotherType>::type>, TValueType, AnotherType>::type
	ComplexChunksOperation(Iterator1<TValueType, X, Y, typename std::common_type<TValueType, AnotherType>::type> lhs,
		Iterator2<AnotherType, N, K, typename std::common_type<TValueType, AnotherType>::type> rhs,
		ValArrayProccessor function)
{
	typename NewMatrixType<X, Y, N, K, Iterator1<TValueType, X, Y, typename std::common_type<TValueType, AnotherType>::type>,
		Iterator2<AnotherType, N, K, typename std::common_type<TValueType, AnotherType>::type>,
		TValueType, AnotherType>::type resultMatrix{};
	for (size_t x_index{}; lhs.isValid(); ++lhs, ++x_index)
	{
		size_t y_index{};
		for (auto it = rhs; it.isValid(); ++it)
		{
			resultMatrix.At(x_index, y_index++) = function(*lhs, *it);
		}
	}
	return resultMatrix;
}