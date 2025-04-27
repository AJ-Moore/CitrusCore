#pragma once 

#include <CCCommon.h>

namespace CitrusCore
{
	//!< Generally used for holding the dimensiosn of a rectangle.(Axis Aligned)
	template <typename T = uint32> class CITRUS_CORE_API Rect {
	public:
		Rect() = default;
		Rect(T x, T y, T width, T height) {
			X = x;
			Y = y;
			Width = width;
			Height = height;
		}

		T X = 0;
		T Y = 0;
		T Width = 0;
		T Height = 0;

		T Left() { return X; }
		T Right() { return Width; }
		T Top() { return Y; }
		T Bottom() { return Height; }
	};

	using Recti = Rect<uint32>;
}