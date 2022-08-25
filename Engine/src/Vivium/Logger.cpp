#include "Logger.h"

namespace Vivium {
#ifdef VIVIUM_EXPOSE_CORE
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
	void __LogBase(const std::string& msg, int line, const char* func_sig, const char* error_severity_text, int severity)
	{
		if (severity < Logger::GetLogLevel()) return;

		// Matches __something and the whitespace after
		// TODO: better cleaner
		//const std::regex cleaner("__\\S+\\s");
		const std::regex cleaner("");
		std::string function_cleaned = std::regex_replace(func_sig, cleaner, "");

		if (Vivium::Flag::Test(VIVIUM_FLAG_ENABLE_FUNCTION_SIGNATURE_LOGGING)) {
			std::cout << std::format(
				"[{}] ({}) {}:{} {}",
				Timer::GetTimeString(),
				error_severity_text,
				function_cleaned,
				line,
				msg
			) << std::endl;
		}
		else {
			std::cout << std::format(
				"[{}] ({}) {}",
				Timer::GetTimeString(),
				error_severity_text,
				msg
			) << std::endl;
		}

		if (error_severity_text == "Fatal") { exit(EXIT_FAILURE); }
	}
	void GLAPIENTRY __GLLogCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		int log_severity = Logger::GetGLSeverity();

		if (log_severity == -1 || log_severity < severity) {
			return;
		}

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
		case GL_DEBUG_SEVERITY_NOTIFICATION:	return "Info";
		case GL_DEBUG_SEVERITY_LOW:				return "Low";
		case GL_DEBUG_SEVERITY_MEDIUM:			return "Medium";
		case GL_DEBUG_SEVERITY_HIGH:			return "High";
		default:								return "InvalidSeverity";
		}
	}

	const char* __GLDebugTypeToString(GLenum type)
	{
		switch (type) {
		case GL_DEBUG_TYPE_ERROR:				return "Error";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "Undefined";
		case GL_DEBUG_TYPE_PORTABILITY:			return "Portability";
		case GL_DEBUG_TYPE_PERFORMANCE:			return "Performance";
		case GL_DEBUG_TYPE_OTHER:				return "Other";
		case GL_DEBUG_TYPE_MARKER:				return "Marker";
		default:								return "InvalidType";
		}
	}

	const char* __GLDebugSourceToString(GLenum source)
	{
		switch (source) {
		case GL_DEBUG_SOURCE_API:				return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "Window";
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "Shader";
		case GL_DEBUG_SOURCE_THIRD_PARTY:		return "Third party";
		case GL_DEBUG_SOURCE_APPLICATION:		return "Application";
		case GL_DEBUG_SOURCE_OTHER:				return "Other";
		default:								return "InvalidSource";
		}
	}
#endif

	int Logger::s_LogLevel = VIVIUM_TRACE;

	void Logger::SetLogLevel(int level)
	{
		s_LogLevel = level;
	}

	int Logger::GetLogLevel()
	{
		return s_LogLevel;
	}

	int Logger::GetGLSeverity()
	{
		switch (s_LogLevel) {
		case VIVIUM_TRACE:
		case VIVIUM_INFO:
			return GL_DEBUG_SEVERITY_LOW;
		case VIVIUM_WARN:
			return GL_DEBUG_SEVERITY_MEDIUM;
		case VIVIUM_ERROR:
			return GL_DEBUG_SEVERITY_HIGH;
		case VIVIUM_FATAL:
			return -1;
		default: return -1;
		}
	}
}
