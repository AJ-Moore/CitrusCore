#pragma once

#include <CCCommon.h>

namespace CitrusCore
{
	namespace PlatformTools
	{
		inline std::string GetPlatformDataPath(std::string path)
		{
#ifdef RENDERER_PS5
			return fs::path("/app0").append(path).string();
#endif
			return path;
		}
	}
}