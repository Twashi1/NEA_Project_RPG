#pragma once

#include "Core.h"
#include "Timer.h"
#include "Vector2.h"

#define VIVIUM_TRACE 0
#define VIVIUM_INFO  1
#define VIVIUM_WARN  2
#define VIVIUM_ERROR 3
#define VIVIUM_FATAL 4

// Only define the following if we're within the Vivium engine
#ifdef __VIVIUM_EXPOSE
	#define LogFatal(msg, ...)	Vivium::__LogBase(std::format(msg, __VA_ARGS__), __LINE__, __FUNCSIG__, "Fatal", VIVIUM_FATAL)
	#define LogError(msg, ...)	Vivium::__LogBase(std::format(msg, __VA_ARGS__), __LINE__, __FUNCSIG__, "Error", VIVIUM_ERROR)
	#define LogWarn(msg, ...)	Vivium::__LogBase(std::format(msg, __VA_ARGS__), __LINE__, __FUNCSIG__, "Warn",  VIVIUM_WARN)
	#define LogInfo(msg, ...)	Vivium::__LogBase(std::format(msg, __VA_ARGS__), __LINE__, __FUNCSIG__, "Info",  VIVIUM_INFO)
	#define LogTrace(msg, ...)	Vivium::__LogBase(std::format(msg, __VA_ARGS__), __LINE__, __FUNCSIG__, "Trace", VIVIUM_TRACE)
#endif
// TODO: change log sensitivity

namespace Vivium {
#ifdef __VIVIUM_EXPOSE
	// TODO shouldn't be API, only doing this for debug
	std::string VIVIUM_API __SignatureToString(const std::string& msg, int line, const char* func_sig);
	void VIVIUM_API __LogBase(const std::string& msg, int line, const char* func_sig, const char* error_severity_message, int severity);
	void GLAPIENTRY __GLLogCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	const char* __GLDebugSeverityToString(GLenum severity);
	const char* __GLDebugTypeToString(GLenum type);
	const char* __GLDebugSourceToString(GLenum source);

	class VIVIUM_API Logger {
	public:
		template <typename T>
		static std::string List(const std::vector<T>& values)
		{
			std::stringstream ss;
			ss << "[";

			for (std::size_t i = 0; i < values.size(); i++) {
				ss << values[i];

				if (i != values.size() - 1) {
					ss << ", ";
				}
			}

			ss << "]";

			return ss.str();
		}

		template <typename T>
		static std::string List(const T* values, const std::size_t& length)
		{
			std::stringstream ss;
			ss << "[";

			for (std::size_t i = 0; i < length; i++) {
				ss << values[i];

				if (i != length - 1) {
					ss << ", ";
				}
			}

			ss << "]";

			return ss.str();
		}

		static void SetLogLevel(int level);
		static int GetLogLevel();
		static int GetGLSeverity();
		static int s_LogLevel;
	};
#endif
}
