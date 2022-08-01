#include "Logger.h"

namespace Vivium {
#ifdef __VIVIUM_EXPOSE
	std::string VIVIUM_API __SignatureToString(const std::string& msg, int line, const char* func_sig)
	{
		// Matches __something and the whitespace after
		const std::regex cleaner("__\\S+\\s");
		std::string function_cleaned = std::regex_replace(func_sig, cleaner, "");

		return std::format(
			"[{}] ASSERTION FAILED: {}:{} {}",
			Timer::GetTimeString(),
			function_cleaned,
			line,
			msg
		);
	}
	void __LogBase(const std::string& msg, int line, const char* func_sig, const char* log_level)
	{
		// Matches __something and the whitespace after
		const std::regex cleaner("__\\S+\\s");
		std::string function_cleaned = std::regex_replace(func_sig, cleaner, "");

		std::cout << std::format(
			"[{}] ({}) {}:{} {}",
			Timer::GetTimeString(),
			log_level,
			function_cleaned,
			line,
			msg
		) << std::endl;

		if (log_level == "Fatal") { exit(EXIT_FAILURE); }
	}
	void GLAPIENTRY __GLLogCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		// TODO set a flag to set notification level
		if (severity > GL_DEBUG_SEVERITY_NOTIFICATION) {
			std::cout << std::format("[{}] ({}) ({}:{}) from {}; {}",
				Timer::GetTimeString(),
				__GLDebugSeverityToString(severity),
				__GLDebugTypeToString(type),
				id,
				__GLDebugSourceToString(source),
				message
			) << std::endl;
		}
	}

	const char* __GLDebugSeverityToString(GLenum severity)
	{
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "Info";
		case GL_DEBUG_SEVERITY_LOW: return "Low";
		case GL_DEBUG_SEVERITY_MEDIUM: return "Medium";
		case GL_DEBUG_SEVERITY_HIGH: return "High";
		default: return "InvalidSeverity";
		}
	}

	const char* __GLDebugTypeToString(GLenum type)
	{
		switch (type) {
		case GL_DEBUG_TYPE_ERROR: return "Error";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined";
		case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
		case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
		case GL_DEBUG_TYPE_OTHER: return "Other";
		case GL_DEBUG_TYPE_MARKER: return "Marker";
		default: return "InvalidType";
		}
	}

	const char* __GLDebugSourceToString(GLenum source)
	{
		switch (source) {
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "Third party";
		case GL_DEBUG_SOURCE_APPLICATION: return "Application";
		case GL_DEBUG_SOURCE_OTHER: return "Other";
		default: return "InvalidSource";
		}
	}
#endif
}
