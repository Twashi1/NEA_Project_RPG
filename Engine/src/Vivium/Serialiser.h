#pragma once

#include "Core.h"
#include "Logger.h"
#include "Vector2.h"

/*
TODO: add functionality for pointers?
TODO: text serialisation
TODO: arrays/containers
*/

namespace Vivium {
	class IStreamable;
	// TODO still ugly
	template <typename T> inline constexpr bool __BaseStreamableTypes = std::is_arithmetic_v<T> || std::is_same_v<T, std::string>;
	template <typename T> concept IsStreamable = std::is_base_of_v<IStreamable, T>
	|| __BaseStreamableTypes<T>
		|| std::is_same_v<T, Vector2<int8_t>>
		|| std::is_same_v<T, Vector2<uint8_t>>
		|| std::is_same_v<T, Vector2<int16_t>>
		|| std::is_same_v<T, Vector2<uint16_t>>
		|| std::is_same_v<T, Vector2<int32_t>>
		|| std::is_same_v<T, Vector2<uint32_t>>
		|| std::is_same_v<T, Vector2<long>>
		|| std::is_same_v<T, Vector2<float>>
		|| std::is_same_v<T, Vector2<double>>;

	class Serialiser;

	class VIVIUM_API Stream {
	private:
		int m_CurrentTabCount = 0;

	public:
		enum class Flag : uint8_t {
			INVALID =	0b00000000,
			BINARY =	0b00000001,
			TEXT =		0b00000010,
			TRUNC =		0b00000100
		};

		static Flag GetMode(const Flag& flag);
		static bool GetTrunc(const Flag& flag);

		std::ofstream* out = nullptr;
		std::ifstream* in = nullptr;

		int GetTabCount() const;
		int IncrementTabCount();
		int DecrementTabCount();
		std::string GetTab() const; // Gives printable string to represent tab space

		Stream();
		~Stream();
	};

	VIVIUM_API int operator|(const Stream::Flag& left, const Stream::Flag& right);
	VIVIUM_API int operator|(const int& left, const Stream::Flag& right);
	VIVIUM_API int operator|(const Stream::Flag& left, const int& right);

	class VIVIUM_API Serialiser {
	private:
		/* WriteBinary */
		template <typename T>
		void m_WriteBinary(Stream& s, const T& data) {
			s.out->write((char*)&data, sizeof(T));
		}

		template <>
		void m_WriteBinary(Stream& s, const std::string& data) {
			s.out->write(data.c_str(), data.length() * sizeof(char));
			s.out->put('\0'); // Write null termination character
		}

		template <typename T>
		void m_WriteBinary(Stream& s, const std::vector<T>& data) {
			unsigned int size = data.size();
			m_WriteBinary<unsigned int>(s, size);

			s.out->write((char*)&data[0], sizeof(T) * size);
		}

		template <>
		void m_WriteBinary(Stream& s, const std::vector<std::string>& data) {
			unsigned int size = data.size();
			m_WriteBinary<unsigned int>(s, size);

			for (unsigned int i = 0; i < size; i++) {
				m_WriteBinary<std::string>(s, data[i]);
			}
		}

		template <typename T>
		void m_WriteBinary(Stream& s, const Vector2<T>& data) {
			s.out->write((char*)&data, sizeof(Vector2<T>));
		}

		/* ReadBinary */
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
		void m_ReadBinary(Stream& s, std::vector<T>* memory) {
			// Get size of vector
			unsigned int size;
			m_ReadBinary<unsigned int>(s, &size);

			char* readbuff = new char[sizeof(T) * size];
			s.in->read(readbuff, sizeof(T) * size);

			memory->resize(size);

			// Alternative &(*memory)[0]
			memcpy(&memory->at(0), readbuff, sizeof(T) * size);

			delete[] readbuff;
		}

		template <>
		void m_ReadBinary(Stream& s, std::vector<std::string>* memory) {
			unsigned int size;
			m_ReadBinary<unsigned int>(s, &size);

			memory->resize(size);

			for (unsigned int i = 0; i < size; i++) {
				m_ReadBinary<std::string>(s, &memory->at(i));
			}
		}

		template <typename T>
		void m_ReadBinary(Stream& s, Vector2<T>* memory) {
			char* readbuff = new char[sizeof(Vector2<T>)];
			s.in->read(readbuff, sizeof(Vector2<T>));

			memcpy(memory, readbuff, sizeof(Vector2<T>));

			delete[] readbuff;
		}

		/* WriteText */
		template <typename T> void m_WriteText(Stream& s, const T& data, const std::string& name) {
			*s.out << s.GetTab() << name << " = " << data << ";" << std::endl;
		}

		template <typename T> void m_WriteText(Stream& s, const Vector2<T>& data, const std::string& name) {
			*s.out << s.GetTab() << name << " = " << "{" << data.x << ", " << data.y << "};" << std::endl;
		}

		template <> void m_WriteText(Stream& s, const std::string& data, const std::string& name) {
			*s.out << s.GetTab() << name << " = \"" << data << "\";" << std::endl;
		}

		template <typename T> void m_WriteText(Stream& s, const std::vector<T>& data, const std::string& name) {
			constexpr bool isMultiLineArray = std::is_base_of_v<IStreamable, T>;

			// TODO: split into functions
			if (isMultiLineArray) {
				// TODO some sort of write string subroutine for stream, maybe takes format
				// maybe just even a #define
				*s.out << s.GetTab() << name << " = [\n";
				
				s.IncrementTabCount();

				for (int i = 0; i < data.size(); i++) {
					*s.out << s.GetTab() << data[i];
					
					if (i == data.size() - 1)
					{
						s.DecrementTabCount();
						*s.out << std::endl << s.GetTab() << "];" << std::endl;
					}
					else { *s.out << ",\n"; }
				}
			}
			else {
				*s.out << s.GetTab() << name << " = [";

				for (int i = 0; i < data.size(); i++) {
					*s.out << data[i];
					if (i == data.size() - 1) { *s.out << "];" << std::endl; }
					else { *s.out << ", "; }
				}
			}
		}

		/* ReadText */
		template <typename T> void m_ReadText(Stream& s, T* memory, const std::string& name) {
			LogError("Reading (T*) data of type {} with name {}", typeid(T).name(), name);

			memory = nullptr;
		}

		template <typename T> void m_ReadText(Stream& s, std::vector<T>* memory, const std::string& name) {
			LogError("Reading vector data of type {} with name {}", typeid(T).name(), name);

			memory = nullptr;
		}

		Stream m_Stream;
		Stream::Flag m_Flags;

	public:
		Serialiser(const Stream::Flag& flags);
		Serialiser(const int& flags);

		void BeginRead(const char* path);
		void BeginWrite(const char* path);

		void EndRead();
		void EndWrite();

		~Serialiser() = default;

		template <typename T>
		void Write(const T& object, const std::string& name = "") {
			if constexpr (!IsStreamable<T>) {
				LogError("Object type {} is not Streamable", typeid(T).name());
			}
			// If the object inherits IStreamable, call IStreamable's write
			else if constexpr (std::is_base_of_v<IStreamable, T>) {
				// If writing text, wrap streamable's data under name of object
				if (Stream::GetMode(m_Flags) == Stream::Flag::TEXT) {
					// NOTE: might make parsing the file more difficult later
					if (!name.empty())
					{
						*m_Stream.out << m_Stream.GetTab() << name << " = {\n";
					}
					else 
					{
						*m_Stream.out << m_Stream.GetTab() << "{\n";
					}

					m_Stream.IncrementTabCount();

					object.Write(*this);

					m_Stream.DecrementTabCount();
					*m_Stream.out << m_Stream.GetTab() << "};\n";
				}
				else {
					object.Write(*this);
				}
			}
			// Its a trivial type
			else {
				switch (Stream::GetMode(m_Flags)) {
				case Stream::Flag::BINARY:
					m_WriteBinary(m_Stream, object); break;
				case Stream::Flag::TEXT:
					m_WriteText(m_Stream, object, name); break;
				default:
					LogError("Invalid stream mode: {}", m_Flags);
				}
			}
		}

		template <typename T>
		void Write(const std::vector<T>& object, const std::string& name = "") {
			if constexpr (!IsStreamable<T>) {
				LogError("Object type {} is not Streamable", typeid(T).name());
			}
			// If the object inherits IStreamable, call IStreamable's write
			else if constexpr (std::is_base_of_v<IStreamable, T>) {
				unsigned int size = object.size();
				// TODO: for unnamed vector, object will just appear as "-size"
				Write<unsigned int>(size, name + "-size");

				LogTrace("Writing size of {}", size);

				for (unsigned int i = 0; i < size; i++) {
					LogTrace("...Writing index {}, value {}", i, object[i]);
					Write<T>(object[i], std::string(i));
				}
			}
			// Its a trivial type
			else {
				switch (Stream::GetMode(m_Flags)) {
				case Stream::Flag::BINARY:
					m_WriteBinary(m_Stream, object); break;
				case Stream::Flag::TEXT:
					m_WriteText(m_Stream, object, name); break;
				default:
					LogError("Invalid stream mode: {}", m_Flags);
				}
			}
		}

		// NOTE: assuming the memory has been allocated for us
		template <typename T>
		void Read(T* memory, const std::string& name = "") {
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
				switch (Stream::GetMode(m_Flags)) {
				case Stream::Flag::BINARY:
					m_ReadBinary(m_Stream, memory); break;
				case Stream::Flag::TEXT:
					m_ReadText(m_Stream, memory, name); break;
				default:
					LogError("Invalid stream mode: {}", m_Flags);
				}
			}
		}

		template <typename T>
		void Read(std::vector<T>* memory, const std::string& name = "") {
			// Ensure object is streamable
			if constexpr (!IsStreamable<T>) {
				LogError("Object type {} is not Streamable", typeid(T).name());
			}
			// If the object inherits IStreamable, call IStreamable's read
			else if constexpr (std::is_base_of_v<IStreamable, T>) {
				unsigned int size;
				Read<unsigned int>(&size, name + "-size");

				memory->resize(size);

				for (unsigned int i = 0; i < size; i++) {
					Read<T>(&memory->at(i), std::string(i));
				}
			}
			// Its a trivial type
			else {
				switch (Stream::GetMode(m_Flags)) {
				case Stream::Flag::BINARY:
					m_ReadBinary(m_Stream, memory); break;
				case Stream::Flag::TEXT:
					m_ReadText(m_Stream, memory, name); break;
				default:
					LogError("Invalid stream mode: {}", m_Flags);
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
	struct formatter<Vivium::Stream::Flag> : formatter<string> {
		auto format(Vivium::Stream::Flag flag, format_context& ctx) {
			std::string value;

			switch (Vivium::Stream::GetMode(flag)) {
			case Vivium::Stream::Flag::BINARY: value = "Binary"; break;
			case Vivium::Stream::Flag::TEXT: value = "Text"; break;
			default: value = "Invalid";
			}
			
			return formatter<string>::format(
				value, ctx);
		}
	};
}