#pragma once

#include <CCCommon.h>
#include <string>

namespace CitrusCore
{
	class CITRUS_CORE_API Logger
	{
	public: 
		enum LogLevel
		{
			DEBUG,
			INFO,
			WARN,
			ERROR,
			FATAL,
			VERBOSE,
		};

		static void Log(LogLevel level, const char* format, ...);
		static void SetLogLevel(LogLevel level) { Instance().m_logLevel = level; }
		static Logger& Instance();

	private:
		void FormatLogMessage(LogLevel level, const std::string& message);
		LogLevel m_logLevel = LogLevel::DEBUG;
	};
}
