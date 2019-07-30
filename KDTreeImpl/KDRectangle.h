#pragma once

#include "KDVector.h"
#include "KDPoint.h"

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
			unsigned distances[DimensionsCount];
			for (unsigned i = 0; i < distances; ++i)
			{
				distances[i] = std::abs(last[i] - first[i]);
			}

		}


	private:
		KDVector<DimensionsCount> first, last;
	};

}