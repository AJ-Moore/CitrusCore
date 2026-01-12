
#include "Math.h"
#include <algorithm>

namespace CitrusCore
{
	double Math::PI = 3.1415926535897932384626433832795;

	float Math::Lerp(float a, float b, float t)
	{
		return std::clamp(a + t * (b - a), 0.0f , 1.0f);
	}

	float Math::MoveTowards(float a, float b, float maxDelta)
	{
		if (a < b)
		{
        	return std::min(a + maxDelta, b);
		}
		
		return std::max(a - maxDelta, b);
	}

	float Math::SmoothLerp(float a, float b, float t, float f = 1)
	{
		return (1 - std::cos(Math::PI * Math::Lerp(a, b, t) * f)) * 0.5f;
	}

	float Math::SmoothLerpTowards(float a, float b, float maxDelta, float f = 1)
	{
		return (1 - std::cos(Math::PI * Math::InverseLerp(a, b, maxDelta) * f)) * 0.5f;
	}

	glm::vec3 Math::Lerp(glm::vec3 a, glm::vec3 b, float t)
	{
		float tClamped = std::clamp(t, 0.0f, 1.0f);
		return a * (1.f - tClamped) + b * tClamped;
	}

	float Math::InverseLerp(float a, float b, float x)
	{
		if (a == b) {
			// Avoid division by zero
			return 0.0f;
		}

		return std::clamp((x - a) / (b - a), 0.0f, 1.0f);
	}
}