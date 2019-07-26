#pragma once

#include <initializer_list>
#include <iomanip>
#include <cstring>
#include <utility>
#include <cmath>

namespace nostd
{

	template <unsigned DimensionsCount>
	class KDPoint
	{
	public:
		KDPoint();
		KDPoint(std::initializer_list<double> i_init_list);

		double& GetCoordinate(unsigned index);

		const double& GetCoordinate(unsigned index) const;

		double DistanceTo(const KDPoint &);

		bool CompareDimensionCoordinates(const KDPoint &other, unsigned index) const;

		void SetValues(std::initializer_list<double> i_init_list);


		std::ostream& Print(std::ostream& stream) const;

		bool operator==(const KDPoint& other)
		{
			for (int i = 0; i < DimensionsCount; ++i)
			{
				if (m_coordinates[i] != other.m_coordinates[i])
					return false;
			}
			return true;
		}

		bool operator!=(const KDPoint& other)
		{
			return !(*this == other);
		}

	private:
		double m_coordinates[DimensionsCount];

	};

	template<unsigned DimensionsCount>
	KDPoint<DimensionsCount>::KDPoint()
	{
		memset(m_coordinates, 0, DimensionsCount * (sizeof(double) / sizeof(int)));
	}

	template<unsigned DimensionsCount>
	KDPoint<DimensionsCount>::KDPoint(std::initializer_list<double> i_init_list)
	{
		SetValues(std::move(i_init_list));
	}


	template<unsigned DimensionsCount>
	double & KDPoint<DimensionsCount>::GetCoordinate(unsigned i_index)
	{
		return m_coordinates[i_index];
	}

	template<unsigned DimensionsCount>
	const double & KDPoint<DimensionsCount>::GetCoordinate(unsigned i_index) const
	{
		return m_coordinates[i_index];
	}

	template<unsigned DimensionsCount>
	double KDPoint<DimensionsCount>::DistanceTo(const KDPoint &otherPoint)
	{
		double accumulation{};
		for (int i = 0; i < DimensionsCount; ++i)
		{
			accumulation += std::pow(otherPoint.m_coordinates[i] - m_coordinates[i]);
		}
		return std::sqrt(accumulation);
	}

	template<unsigned DimensionsCount>
	bool KDPoint<DimensionsCount>::CompareDimensionCoordinates(const KDPoint<DimensionsCount> & other, unsigned index) const
	{
		return m_coordinates[index] < other.m_coordinates[index];
	}

	template<unsigned DimensionsCount>
	void KDPoint<DimensionsCount>::SetValues(std::initializer_list<double> i_init_list)
	{
		if (i_init_list.size() != DimensionsCount)
			throw std::invalid_argument{ "Invalid initializer list size" };
		size_t current_index{};
		for (auto &element : i_init_list)
		{
			m_coordinates[current_index++] = std::move(element);
		}
	}

	template<unsigned DimensionsCount>
	std::ostream & KDPoint<DimensionsCount>::Print(std::ostream & stream) const
	{
		auto flags = stream.setf(std::ios::fixed);
		stream << "{ ";
		for (unsigned i = 0; i < DimensionsCount - 1; ++i)
		{
			stream << std::setprecision(1) << m_coordinates[i] << ", ";
		}
		stream << std::setprecision(1) << m_coordinates[DimensionsCount - 1] << " }";
		stream.setf(flags);

		return stream;
	}

	template <unsigned DimensionsCount>
	std::ostream & operator<<(std::ostream & stream, const KDPoint<DimensionsCount> & out)
	{
		return out.Print(stream);
	}

	template <unsigned DimensionsCount>
	class PointsComparator
	{
	public:
		PointsComparator() = default;

		PointsComparator(unsigned coordinateToCompare)
			:sortingIndex{coordinateToCompare}
		{}

		bool operator()(const KDPoint<DimensionsCount> &lhs, const KDPoint<DimensionsCount> &rhs)
		{
			return  lhs.CompareDimensionCoordinates(rhs, sortingIndex);
		}

		PointsComparator& operator++()
		{
			sortingIndex = (sortingIndex + 1) % DimensionsCount;
			return *this;
		}

		unsigned GetCurrentIndex() const
		{
			return sortingIndex;
		}

		static PointsComparator& GetComparator()
		{
			mg_comparator.sortingIndex = 0;
			return mg_comparator;
		}
	private:
		static PointsComparator<DimensionsCount> mg_comparator;
		unsigned sortingIndex = 0;
	};

	template <unsigned DimensionsCount>
	PointsComparator<DimensionsCount> PointsComparator<DimensionsCount>::mg_comparator{};

}