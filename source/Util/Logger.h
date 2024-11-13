#pragma once

#include <CCCommon.h>
#include <string>
#include <cstdarg>

namespace CitrusCore
{
	CITRUS_CORE_API class Logger
	{
	public: 
		enum LogLevel
		{
			DEBUG,
			INFO,
			WARN,
			ERROR,
			FATAL
		};

		static void Log(LogLevel level, const char* format, ...);
		static void SetLogLevel(LogLevel level) { Instance().m_logLevel = level; }
		static Logger& Instance();

	private:
		void FormatLogMessage(LogLevel level, const std::string& message);
		LogLevel m_logLevel = LogLevel::DEBUG;
	};

	extern "C" void Log(Logger::LogLevel level, const char* format, ...) {
		va_list args;
		va_start(args, format);
		Logger::Log(level, format, args);
		va_end(args);
	}
}
