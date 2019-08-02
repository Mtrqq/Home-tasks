#pragma once

#include "KDVector.h"
#include "KDPoint.h"

#include <vector>

namespace nostd
{
	template <unsigned DimensionsCount>
	class KDRectangle
	{
	public:
		KDRectangle() = default;

		KDRectangle(const KDPoint<DimensionsCount>& lhs, const KDPoint<DimensionsCount>& rhs)
		{
			for (unsigned i = 0; i < DimensionsCount; ++i)
			{
				first[i] = lhs.CoordinateAt(i);
				last[i] = rhs.CoordinateAt(i);
			}
		}

		KDRectangle(const KDVector<DimensionsCount>& lhs, const KDVector<DimensionsCount>& rhs)
			:first{ lhs }, last{ rhs } 
		{}

		std::vector<KDVector<DimensionsCount>> GetVertices() const
		{
			auto distances = last - first;
			std::vector<KDVector<DimensionsCount>> result;
			GenerateVertices(result, distances, first);
			return result;
		}

		bool Contains(const KDPoint<DimensionsCount>& point)
		{
			for (unsigned i = 0; i < DimensionsCount; ++i)
			{
				if (point.At(i) < first.At(i) || point.At(i) > last.At(i))
				{
					return false;
				}
			}
			return true;
		}

		bool Overlap(const KDRectangle<DimensionsCount> &other);

		std::pair<KDRectangle<DimensionsCount>, KDRectangle<DimensionsCount>>
			SplitByAxis(unsigned dimmension, double value);


	private:
		KDVector<DimensionsCount> first, last;

		void GenerateVertices(std::vector<KDVector<DimensionsCount>> &io_vec,
						      const KDVector<DimensionsCount> &distances,
							  KDVector<DimensionsCount> current,
							  unsigned index = 0);

		std::pair<int, int> GetAxisProjection(const std::vector<KDVector<DimensionsCount>> &vertices,
											  const KDVector<DimensionsCount> &axis)
		{
			auto min = axis.DotProduct(vertices.front());
			auto max = min;
			for (unsigned i = 1; i < vertices.size(); ++i)
			{
				auto actual = axis.DotProduct(vertices[i]);
				if (actual > max)
					max = actual;
				else
					if (actual < min)
						min = actual;
			}
			return std::make_pair(min, max);
		}
	};

	template<unsigned DimensionsCount>
	bool KDRectangle<DimensionsCount>::Overlap(const KDRectangle<DimensionsCount>& other)
	{
		KDVector<DimensionsCount> Axis;
		auto self_vertices = GetVertices();
		auto other_vertices = other.GetVertices();
		for (unsigned i = 0; i < DimensionsCount; ++i)
		{
			Axis.At(i) = 1;
			auto projection_f = GetAxisProjection(self_vertices, Axis);
			auto projection_s = GetAxisProjection(other_vertices, Axis);
			if (projection_f.first > projection_s.second ||
				projection_s.first > projection_f.second)
				return false;
		}
		return true;
	}

	template<unsigned DimensionsCount>
	std::pair<KDRectangle<DimensionsCount>, KDRectangle<DimensionsCount>> KDRectangle<DimensionsCount>::SplitByAxis(unsigned dimmension, double value)
	{
		KDVector<DimensionsCount> right_middle = first;
		right_middle.At(dimmension) = value;
		KDVector<DimensionsCount> left_middle = right;
		left_middle.At(dimmension) = value;
		return std::make_pair({ first, left_middle }, { right_middle, last });
	}

	template<unsigned DimensionsCount>
	void KDRectangle<DimensionsCount>::GenerateVertices(std::vector<KDVector<DimensionsCount>>& io_vec,
														const KDVector<DimensionsCount>& distances, 
														KDVector<DimensionsCount> current,
														unsigned index)
	{
		if (index == DimensionsCount)
		{
			io_vec.push_back(current);
		}
		else
		{
			GenerateVertices(io_vec, distances, current, index + 1);
			current.At(index) = distances.At(index);
			GenerateVertices(io_vec, distances, current, index + 1);
		}
	}

}