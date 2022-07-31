#pragma once

#include "Core.h"
#include "Timer.h"

// Only define the following if we're within the Vivium engine
#ifdef __VIVIUM_EXPOSE
	#define LogFatal(msg, ...)	Vivium::__LogBase(std::format(msg, __VA_ARGS__), __LINE__, __FUNCSIG__, "Fatal"); exit(EXIT_FAILURE)
	#define LogError(msg, ...)	Vivium::__LogBase(std::format(msg, __VA_ARGS__), __LINE__, __FUNCSIG__, "Error")
	#define LogWarn(msg, ...)	Vivium::__LogBase(std::format(msg, __VA_ARGS__), __LINE__, __FUNCSIG__, "Warn")
	#define LogInfo(msg, ...)	Vivium::__LogBase(std::format(msg, __VA_ARGS__), __LINE__, __FUNCSIG__, "Info")
	#define LogTrace(msg, ...)	Vivium::__LogBase(std::format(msg, __VA_ARGS__), __LINE__, __FUNCSIG__, "Trace")

	#define Assert(condition, msg, ...) if (!condition) { Vivium::__LogBase(std::format(msg, __VA_ARGS__), __LINE__, __FUNCSIG__, "ASSERT FAIL"); }
#endif

// TODO: change log sensitivity

namespace Vivium {
#ifdef __VIVIUM_EXPOSE
	// TODO shouldn't be API, only doing this for debug
	void VIVIUM_API __LogBase(const std::string& msg, int line, const char* func_sig, const char* log_level);
	void GLAPIENTRY __GLLogCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	const char* __GLDebugSeverityToString(GLenum severity);
	const char* __GLDebugTypeToString(GLenum type);
	const char* __GLDebugSourceToString(GLenum source);
#endif
}
