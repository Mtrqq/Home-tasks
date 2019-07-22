#pragma once

#include "MatrixWalker.h"
#include "MatrixHelperFunctions.h"

#include <initializer_list>
#include <type_traits>
#include <functional>
#include <array>

template <typename TValueType, size_t X, size_t Y>
class Matrix
{
public:
	Matrix() = default;

	Matrix(std::initializer_list<std::initializer_list<TValueType>> matrix);

	explicit Matrix(const TValueType& filler) { Fill(filler); }

	template <typename AnotherType>
	Matrix(const Matrix<AnotherType, X, Y>& another);

	template <typename AnotherType>
	Matrix(Matrix<AnotherType, X, Y>&& another) noexcept;

	constexpr size_t Width() const { return Y; }

	constexpr size_t Height() const { return X; }

	void Fill(const TValueType& filler);

	template <typename UnaryFunction>
	Matrix<TValueType, X, Y>& Apply(const UnaryFunction& function);

	template <typename UnaryFunction>
	Matrix<TValueType, X, Y> EvaluateUnary(const UnaryFunction& function) const;

	template <typename BinaryFunction, typename AnotherType>
	Matrix<typename  std::common_type<TValueType, AnotherType>::type, X, Y>
		EvaluateBinary(const Matrix<AnotherType, X, Y>&, const BinaryFunction& function) const;

	Matrix<TValueType, X, Y> Trasposed() const;

	typename std::enable_if<X == Y && std::is_arithmetic<TValueType>::value, TValueType>::type Determinant() const;

	const TValueType& At(size_t x_index, size_t y_index) const;

	TValueType& At(size_t x_index, size_t y_index)
	{
		const Matrix& self = *this;
		return const_cast<TValueType&>(self.At(x_index, y_index));
	}

	const TValueType* operator[](size_t x_index) const
	{
		return m_container[x_index];
	}

	TValueType* operator[](size_t x_index)
	{
		const Matrix& self = *this;
		return const_cast<TValueType*>(self.operator[](x_index));
	}

	std::ostream& Print(std::ostream& stream) const;

	template <typename AnotherType, size_t N, size_t K>
	auto operator*(const Matrix<AnotherType, N, K>& anotherMatrix) const;

	template <typename AnotherType>
	auto operator+(const Matrix<AnotherType, X, Y>& anotherMatrix) const;

	template <typename AnotherType>
	auto operator-(const Matrix<AnotherType, X, Y>& anotherMatrix) const;

	template <typename AnotherType>
	typename std::enable_if<std::is_arithmetic<AnotherType>::value, Matrix<typename std::common_type<TValueType,AnotherType>::type,X,Y>>::type
		operator*(const AnotherType& value) const;

	template <typename TValueType, typename AnotherType, size_t X, size_t Y>
	friend typename std::enable_if<std::is_arithmetic<AnotherType>::value, Matrix<typename std::common_type<TValueType, AnotherType>::type, X, Y>>::type
		operator*(const AnotherType& value, const Matrix<TValueType, X, Y>& matrix);

	template <typename T, size_t N, size_t K>
	friend std::ostream& operator<<(std::ostream&, const Matrix<T, N, K>&);

	template <typename NeededTypename>
	ColumnWalker<TValueType, X, Y, NeededTypename> GetColumnWalker(size_t starting_column = 0) const
	{
		return ColumnWalker<TValueType, X, Y, NeededTypename>{ *this, starting_column };
	}

	template <typename NeededTypename>
	RowWalker<TValueType, X, Y, NeededTypename> GetRowWalker(size_t starting_row = 0) const
	{
		return RowWalker<TValueType, X, Y, NeededTypename>{ *this, starting_row };
	}

private:
	TValueType m_container[X][Y];
};

template<typename TValueType, size_t X, size_t Y>
template<typename AnotherType>
Matrix<TValueType, X, Y>::Matrix(const Matrix<AnotherType, X, Y>& another)
{
	for (int i = 0; i < X; ++i)
		for (int j = 0; j < Y; ++j)
			m_container[i][j] = another.m_container[i][j];
}

template<typename TValueType, size_t X, size_t Y>
template<typename AnotherType>
Matrix<TValueType, X, Y>::Matrix(Matrix<AnotherType, X, Y>&& another) noexcept
	:m_container{}
{
	for (int i = 0; i < X; ++i)
		for (int j = 0; j < Y; ++j)
			m_container[i][j] = std::move(another.m_container[i][j]);
}

template<typename TValueType, size_t X, size_t Y>
Matrix<TValueType, X, Y>::Matrix(std::initializer_list<std::initializer_list<TValueType>> matrix)
	:m_container{}
{
	if (matrix.size() != X || (*matrix.begin()).size() != Y)
		throw std::invalid_argument{ "invalid size of initializer list" };
	size_t x_index{};
	for (auto& row : matrix)
	{
		size_t y_index{};
		for (auto& element : row)
		{
			m_container[x_index][y_index++] = std::move(element);
		}
		++x_index;
	}
}

template<typename TValueType, size_t X, size_t Y>
std::ostream& Matrix<TValueType, X, Y>::Print(std::ostream& stream) const
{
	for (size_t i = 0; i < X; ++i)
	{
		for (size_t j = 0; j < Y; ++j)
		{
			stream << m_container[i][j] << '\t';
		}
		stream << std::endl;
	}
	return stream;
}

template<typename TValueType, size_t X, size_t Y>
void Matrix<TValueType, X, Y>::Fill(const TValueType& filler)
{
	for (size_t i = 0; i < X; ++i)
		for (size_t j = 0; j < Y; ++j)
			m_container[i][j] = filler;
}

template<typename TValueType, size_t X, size_t Y>
Matrix<TValueType, X, Y> Matrix<TValueType, X, Y>::Trasposed() const
{
	Matrix<TValueType, X, Y> resultMatrix;
	for (size_t i = 0; i < X; ++i)
	{
		for (size_t j = 0; j < Y; ++j)
		{
			resultMatrix[i][j] = m_container[j][i];
		}
	}
	return resultMatrix;
}

template<typename TValueType, size_t X, size_t Y>
template<typename UnaryFunction>
Matrix<TValueType, X, Y>& Matrix<TValueType, X, Y>::Apply(const UnaryFunction& function)
{
	for (int i = 0; i < X; ++i)
	{
		for (int j = 0; j < Y; ++j)
		{
			m_container[i][j] = function(m_container[i][j]);
		}
	}
	return *this;
}

template<typename TValueType, size_t X, size_t Y>
template<typename UnaryFunction>
Matrix<TValueType,X,Y> Matrix<TValueType, X, Y>::EvaluateUnary(const UnaryFunction& function) const
{
	Matrix<TValueType, X, Y> result;
	for (int i = 0; i < X; ++i)
	{
		for (int j = 0; j < Y; ++j)
		{
			result[i][j] = function(m_container[i][j]);
		}
	}
	return result;
}

template<typename TValueType, size_t X, size_t Y>
template<typename BinaryFunction, typename AnotherType>
Matrix<typename std::common_type<TValueType, AnotherType>::type, X, Y>
Matrix<TValueType, X, Y>::EvaluateBinary(const Matrix<AnotherType, X, Y>& another, const BinaryFunction& function) const
{
	Matrix<typename std::common_type<TValueType, AnotherType>::type, X, Y> resultMatrix;
	for (size_t i = 0; i < resultMatrix.Height(); ++i)
	{
		for (size_t j = 0; j < resultMatrix.Width(); ++j)
		{
			resultMatrix.At(i, j) = function(At(i, j), another.At(i, j));
		}
	}
	return resultMatrix;
}

template<typename TValueType, size_t X, size_t Y>
template<typename AnotherType, size_t N, size_t K>
auto Matrix<TValueType, X, Y>::operator*(const Matrix<AnotherType, N, K>& anotherMatrix) const
{
	using common_type = typename std::common_type<TValueType, AnotherType>::type;
	return ComplexChunksOperation(GetRowWalker<common_type>(), anotherMatrix.GetColumnWalker<common_type>(),
		[](const std::valarray<common_type>& lhs, const std::valarray<common_type>& rhs)
		-> common_type
		{
			return (lhs * rhs).sum();
		});
}

template<typename TValueType, size_t X, size_t Y>
template<typename AnotherType>
auto Matrix<TValueType, X, Y>::operator+(const Matrix<AnotherType, X, Y>& anotherMatrix) const
{
	return EvaluateBinary(anotherMatrix, std::plus<typename std::common_type<TValueType, AnotherType>::type>{});
}

template<typename TValueType, size_t X, size_t Y>
template<typename AnotherType>
auto Matrix<TValueType, X, Y>::operator-(const Matrix<AnotherType, X, Y>& anotherMatrix) const
{
	return EvaluateBinary(anotherMatrix, std::minus<typename std::common_type<TValueType, AnotherType>::type>{});
}

template<typename TValueType, size_t X, size_t Y>
template<typename AnotherType>
typename std::enable_if<std::is_arithmetic<AnotherType>::value, Matrix<typename std::common_type<TValueType, AnotherType>::type, X, Y>>::type 
Matrix<TValueType, X, Y>::operator*(const AnotherType& value) const
{
	return EvaluateUnary(std::bind(std::multiplies<typename std::common_type<TValueType, AnotherType>::type>{}, std::placeholders::_1, value));
}

template<typename TValueType, typename AnotherType, size_t X, size_t Y>
typename std::enable_if<std::is_arithmetic<AnotherType>::value, Matrix<typename std::common_type<TValueType, AnotherType>::type, X, Y>>::type operator*(const AnotherType& value, const Matrix<TValueType, X, Y>& matrix)
{
	return matrix * value;
}

template<typename T, size_t N, size_t K>
std::ostream& operator<<(std::ostream& stream, const Matrix<T, N, K>& matrix)
{
	return matrix.Print(stream);
}


template<typename TValueType, size_t X, size_t Y>
typename std::enable_if<X == Y && std::is_arithmetic<TValueType>::value, TValueType>::type Matrix<TValueType, X, Y>::Determinant() const
{
	TValueType L[X][Y], U[X][Y];
	for (size_t i = 0; i < X; ++i)
	{
		for (size_t k = 0; k < X; ++k)
		{
			TValueType accumulation{};
			for (size_t j = 0; j < i; ++j)
			{
				accumulation += (L[i][j] * U[j][k]);
			}
			U[i][k] = m_container[i][k] - accumulation;
			if (i == k) L[k][k] = 1;
			else L[k][i] = (m_container[k][i] - accumulation) / U[i][i];
		}
	}
	TValueType result{ 1 };
	for (int i = 0; i < X; ++i)
	{
		result *= (L[i][i] * U[i][i]);
	}
	return result;
}

template<typename TValueType, size_t X, size_t Y>
const TValueType& Matrix<TValueType, X, Y>::At(size_t x_index, size_t y_index) const
{
	if (x_index < X && x_index >= 0 && y_index < Y && y_index >= 0)
		return m_container[x_index][y_index];
	throw std::out_of_range{ "index out of range" };
}

