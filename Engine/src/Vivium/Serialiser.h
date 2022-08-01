#pragma once

#include "Core.h"
#include "Logger.h"
#include "Vector2.h"

/*
TODO: add functionality for pointers?
TODO: text serialisation
TODO: vectors/arrays/containers
*/

namespace Vivium {
	class IStreamable;
	// TODO: could simplify into a arithmetic and Vector2<arithmetic>, but that makes it a bit more difficult to understand
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

	class Serialiser;

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
		// TODO: write requires IsStreamable<T> for clarity's sake? maybe excessive
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

		// TODO: slow for long strings
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
		T m_ReadBinary(Stream& s) {
			static_assert(std::is_default_constructible_v<T>, "Object does not have default constructor");

			char* readbuff = new char[sizeof(T)];
			s.in->read(readbuff, sizeof(T));

			T instance;

			memcpy(&instance, readbuff, sizeof(T));

			delete[] readbuff;

			return instance;
		}

		// TODO: slow for long strings
		template <>
		std::string m_ReadBinary(Stream& s) {
			char lastchar;

			std::string instance;

			s.in->get(lastchar); // Get first character
			instance += lastchar; // Add to instance

			// While the last character is not null termination character
			while (lastchar != '\0') {
				s.in->get(lastchar); // Get next character
				instance += lastchar; // Add next character to instance

				// If character is null termination character, break
				if (lastchar == '\0') break;
			}

			return instance;
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

		template <typename T>
		T m_WriteText(Stream& s) {
			LogError("Serialising text not supported yet");
			
			return NULL;
		}

		template <typename T>
		T m_ReadText(Stream& s) {
			LogError("Unserialising text not supported yet");

			return NULL;
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
		void Write(const T& object) {
			if constexpr (!IsStreamable<T>) {
				LogError("Object type {} is not Streamable", typeid(T).name());
			}
			// If the object inherits IStreamable, call IStreamable's write
			else if constexpr (std::is_base_of_v<IStreamable, T>) {
				object.Write(*this);
			}
			// Its a trivial type
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
		void Read(T* memory)  {
			// Ensure object is streamable
			if constexpr (!IsStreamable<T>) {
				LogError("Object type {} is not Streamable", typeid(T).name());
			}
			// If the object inherits IStreamable, call IStreamable's read
			else if constexpr (std::is_base_of_v<IStreamable, T>) {
				memory->Read(*this);
			}
			// Its a trivial type
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

		template <typename T>
		T Read() {
			// Ensure object is streamable
			if constexpr (!IsStreamable<T>) {
				LogError("Object type {} is not Streamable", typeid(T).name());
			}
			// If the object inherits IStreamable, call IStreamable's read
			else if constexpr (std::is_base_of_v<IStreamable, T>) {
				static_assert(std::is_default_constructible_v<T>, "Object does not have default constructor, use void Read(T*)");

				T instance;

				instance.Read(*this);

				return instance;
			}
			// Its a trivial type
			else {
				switch (m_Mode) {
				case Stream::Mode::BINARY:
					return m_ReadBinary<T>(m_Stream);
				case Stream::Mode::TEXT:
					return m_ReadText<T>(m_Stream);
				default:
					LogError("Invalid stream mode: {}", m_Mode); return NULL;
				}
			}
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