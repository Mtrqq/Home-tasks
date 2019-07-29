#pragma once

#include "KDVector.h"

namespace nostd
{

	template <unsigned DimensionsCount>
	class KDRectangle
	{
	public:
		KDRectangle()
	private:
		KDVector<DimensionsCount> first, last;
	};

}