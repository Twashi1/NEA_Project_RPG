#pragma once

#include "Core.h"
#include "Logger.h"
#include "Vector2.h"

/*
TODO: add functionality for pointers?
*/

namespace Vivium {
	class IStreamable;
	// TODO must be a better way
	template <typename T> concept IsStreamable = std::is_base_of_v<IStreamable, T>
		|| std::is_same_v<T, uint8_t>
		|| std::is_same_v<T, int8_t>
		|| std::is_same_v<T, uint16_t>
		|| std::is_same_v<T, int16_t>
		|| std::is_same_v<T, uint32_t>
		|| std::is_same_v<T, int32_t>
		|| std::is_same_v<T, long>
		|| std::is_same_v<T, float>
		|| std::is_same_v<T, double>
		|| std::is_same_v<T, Vector2<uint8_t>>
		|| std::is_same_v<T, Vector2<int8_t>>
		|| std::is_same_v<T, Vector2<uint16_t>>
		|| std::is_same_v<T, Vector2<int16_t>>
		|| std::is_same_v<T, Vector2<uint32_t>>
		|| std::is_same_v<T, Vector2<int32_t>>
		|| std::is_same_v<T, Vector2<long>>
		|| std::is_same_v<T, Vector2<float>>
		|| std::is_same_v<T, Vector2<double>>
		|| std::is_same_v<T, std::string>;

	class VIVIUM_API Serialiser;

	class VIVIUM_API Stream {
	public:
		enum class Mode : uint8_t {
			INVALID,
			BINARY,
			TEXT
		};

		std::ofstream* out = nullptr;
		std::ifstream* in = nullptr;

		Stream();
		~Stream();
	};

	class VIVIUM_API Serialiser {
	private:
		// TODO: decide whether to write requires IsStreamable<T> for clarity's sake
		template <typename T>
		void m_WriteBinary(Stream& s, const T& data) {
			s.out->write((char*)&data, sizeof(T));
		}

		template <>
		void m_WriteBinary(Stream& s, const std::string& data) {
			s.out->write(data.c_str(), data.length() * sizeof(char));
			s.out->put('\0'); // Write null termination character
		}

		// Assuming memory has already been allocated
		// TODO: since this function will only be run for PODs, we could use a default constructor to allocate mem
		// maybe check if memory is already allocated? how would you make the distinction?
		template <typename T>
		void m_ReadBinary(Stream& s, T* memory) {
			char* readbuff = new char[sizeof(T)]; // Allocate buffer for file data
			s.in->read(readbuff, sizeof(T)); // Read file data into buffer

			memcpy(memory, readbuff, sizeof(T)); // Copy data from buffer into pointer given

			delete[] readbuff; // Delete buffer after copying the data
		}

		// Memory for string doesn't have to be allocated here, but for the template it does...
		template <>
		void m_ReadBinary(Stream& s, std::string* memory) {
			char lastchar;

			s.in->get(lastchar); // Get first character
			*memory += lastchar; // Add to memory

			// While the last character is not null termination character
			while (lastchar != '\0') {
				s.in->get(lastchar); // Get next character
				*memory += lastchar; // Add next character to memory

				// If character is null termination character, break
				if (lastchar == '\0') break;
			}
		}

		template <typename T>
		void m_WriteText(Stream& s, const T& data) {
			LogError("Serialising text not supported yet");
		}

		template <typename T>
		void m_ReadText(Stream& s, T* memory) {
			LogError("Unserialising text not supported yet");

			memory = nullptr;
		}

		Stream m_Stream;
		Stream::Mode m_Mode;

	public:
		Serialiser(const Stream::Mode& mode);

		void BeginRead(const char* path);
		void BeginWrite(const char* path);

		void EndRead();
		void EndWrite();

		~Serialiser() = default;

		template <typename T>
		void Write(const T& object) requires IsStreamable<T> {
			// If the object inherits IStreamable, recursively attempt to Write
			if constexpr (std::is_base_of_v<IStreamable, T>) {
				object.Write(*this);
			}
			// TODO: maybe the two separate methods can be combined into one?
			// We got to a trivial type
			else {
				switch (m_Mode) {
				case Stream::Mode::BINARY:
					m_WriteBinary(m_Stream, object); break;
				case Stream::Mode::TEXT:
					m_WriteText(m_Stream, object); break;
				default:
					LogError("Invalid stream mode: {}", m_Mode);
				}
			}
		}

		// NOTE: assuming the memory has been allocated for us
		template <typename T>
		void Read(T* memory) requires IsStreamable<T> {
			// If the object inherits IStreamable, recursively attempt to Read
			if constexpr (std::is_base_of_v<IStreamable, T>) {
				memory->Read(*this);
			}
			// We got to a trivial type
			else {
				switch (m_Mode) {
				case Stream::Mode::BINARY:
					m_ReadBinary(m_Stream, memory); break;
				case Stream::Mode::TEXT:
					m_ReadText(m_Stream, memory); break;
				default:
					LogError("Invalid stream mode: {}", m_Mode);
				}
			}
		}

		// TODO: since im separating these functions only by concept, shouldn't there just be a conditional constexpr?
		// Will run if T is not Streamable
		template <typename T>
		void Write(const T& object) {
			LogError("Object type {} is not Streamable", typeid(T).name());
		}

		// Will run if T is not Streamable
		template <typename T>
		void Read(T* memory) {
			LogError("Object type {} is not Streamable", typeid(T).name());
		}
	};

	class VIVIUM_API IStreamable {
	public:
		virtual void Write(Serialiser& s) const = 0;
		virtual void Read(Serialiser& s) = 0;

		IStreamable() = default;
		virtual ~IStreamable() = default;
	};
}

namespace std {
	template <>
	struct formatter<Vivium::Stream::Mode> : formatter<string> {
		auto format(Vivium::Stream::Mode mode, format_context& ctx) {
			std::string value;

			switch (mode) {
			case Vivium::Stream::Mode::BINARY: value = "Binary"; break;
			case Vivium::Stream::Mode::TEXT: value = "Text"; break;
			default: value = "Invalid";
			}
			
			return formatter<string>::format(
				value, ctx);
		}
	};
}