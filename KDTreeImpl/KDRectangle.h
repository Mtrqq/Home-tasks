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
				first[i] = lhs.At(i);
				last[i] = rhs.At(i);
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

		bool Contains(const KDPoint<DimensionsCount>& point) const
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

		bool Overlap(const KDRectangle<DimensionsCount>& other) const;

		std::pair<KDRectangle<DimensionsCount>, KDRectangle<DimensionsCount>>
			SplitByAxis(unsigned dimmension, double value) const;


	private:
		KDVector<DimensionsCount> first, last;

		void GenerateVertices(std::vector<KDVector<DimensionsCount>>& io_vec,
			const KDVector<DimensionsCount>& distances,
			KDVector<DimensionsCount> current,
			unsigned index = 0) const;

		// SPEED UP WITHOUT MUL
		std::pair<double, double> GetAxisProjection(const std::vector<KDVector<DimensionsCount>>& vertices, unsigned dimmension) const
		{
			auto min = vertices.front().At(dimmension);
			auto max = min;
			for (unsigned i = 1; i < vertices.size(); ++i)
			{
				auto actual = vertices[i].At(dimmension);
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
	bool KDRectangle<DimensionsCount>::Overlap(const KDRectangle<DimensionsCount>& other) const
	{
		auto self_vertices = GetVertices();
		auto other_vertices = other.GetVertices();
		for (unsigned axis = 0; axis < DimensionsCount; ++axis)
		{
			auto projection_f = GetAxisProjection(self_vertices, axis);
			auto projection_s = GetAxisProjection(other_vertices, axis);
			if (projection_f.first > projection_s.second ||
				projection_s.first > projection_f.second)
				return false;
		}
		return true;
	}

	template<unsigned DimensionsCount>
	std::pair<KDRectangle<DimensionsCount>, KDRectangle<DimensionsCount>> KDRectangle<DimensionsCount>::SplitByAxis(unsigned dimmension, double value) const
	{
		KDVector<DimensionsCount> right_middle = first;
		right_middle.At(dimmension) = value;
		KDVector<DimensionsCount> left_middle = last;
		left_middle.At(dimmension) = value - 0.001;
		return std::make_pair(KDRectangle<DimensionsCount>{ first, left_middle }
		, KDRectangle<DimensionsCount>{ right_middle, last });
	}

	template<unsigned DimensionsCount>
	void KDRectangle<DimensionsCount>::GenerateVertices(std::vector<KDVector<DimensionsCount>>& io_vec,
		const KDVector<DimensionsCount>& distances,
		KDVector<DimensionsCount> current,
		unsigned index) const
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