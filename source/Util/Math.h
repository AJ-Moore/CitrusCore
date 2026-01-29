#pragma once
#include <CCCommon.h>
#include <glm/glm.hpp>

namespace CitrusCore {
	class CITRUS_CORE_API Math {
	public: 
		static double PI;
		static float Lerp(float a, float b, float t);
		static float MoveTowards(float a, float b, float maxDelta);
		static float SmoothLerp(float a, float b, float t, float f);
		static float SmoothLerpTowards(float a, float b, float maxDelta, float f);
		static glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float t);
		static float InverseLerp(float a, float b, float x);
		static float FracMod(float f) { return f - ::floorf(f); }
		static float MaxF(float a, float b) { return a > b ? a : b; }
		static float MinF(float a, float b) { return a < b ? a : b; }
		static float Abs(float a) { if (a) return a; else return -a; }
	};
}
