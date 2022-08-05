#pragma once

#include "Core.h"
#include "Logger.h"
#include "Vector2.h"
#include "Utils.h"

/*
TODO: add functionality for pointers?
TODO: text serialisation
TODO: arrays/containers
TODO: Ignore name in Read
TODO: Cleanup
TODO: split IStreamable, Serialiser, Stream into seperate files
*/

namespace Vivium {
	class IStreamable;
	// TODO still ugly
	template <typename T> inline constexpr bool __BaseStreamableTypes = std::is_arithmetic_v<T> || std::is_same_v<T, std::string>;
	template <typename T> concept IsBaseStreamable = __BaseStreamableTypes<T>;
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
		int m_ScopeLevel = 0;

	public:
		enum Flags : int {
			TEXT = NULL,
			BINARY = std::ios::binary,
			TRUNC = std::ios::trunc,
			APPEND = std::ios::app
		};

		std::ofstream* out = nullptr;
		std::ifstream* in = nullptr;

		int GetScopeLevel() const;
		int IncrementScope();
		int DecrementScope();
		std::string GetTab() const; // Gives printable string to represent tab space

		Stream();
		~Stream();
	};

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

		template <typename T>
		void m_WriteText(Stream& s, const std::vector<T>& data) {
			constexpr bool isMultiLineArray = std::is_base_of_v<IStreamable, T>;

			// TODO: split into functions
			if (isMultiLineArray) {
				// TODO some sort of write string subroutine for stream, maybe takes format
				// maybe just even a #define
				*s.out << s.GetTab() << "" << " = [\n";
				
				s.IncrementScope();

				for (int i = 0; i < data.size(); i++) {
					Write<T>(data[i]);
					
					if (i == data.size() - 1)
					{
						s.DecrementScope();
						*s.out << std::endl << s.GetTab() << "];" << std::endl;
					}
					else { *s.out << ",\n"; }
				}
			}
			else {
				*s.out << s.GetTab() << "" << " = [";

				for (int i = 0; i < data.size(); i++) {
					*s.out << data[i];
					if (i == data.size() - 1) { *s.out << "];" << std::endl; }
					else { *s.out << ", "; }
				}
			}
		}

		/* ReadText */
		template <typename T>
		void m_ReadText(Stream& s, T* memory) requires IsBaseStreamable<T>
		{
			// Matches an equal sign as long as its not in quotes, and then the whitespace after the equal if its there
			const std::regex match_equal("^[^\"].*?= ?");
			// Matches ; and then newline at tend of statement
			const std::regex end_of_statement("; ?(\\n)?$");

			std::string statement;
			std::getline(*s.in, statement);

			if (statement.empty()) {
				LogError("Got empty statement from file? Invalid text file");
				memory = nullptr;
				return;
			}

			// Strip the name of the value, and the "="
			std::string expr = std::regex_replace(statement, match_equal, "");
			// Strip end of line characters
			expr = std::regex_replace(expr, end_of_statement, "");

			// For casting to T
			std::istringstream iss(expr);

			// Get from string stream and push to expr
			iss >> *memory;
		}

		// TODO: Read string

		template <typename T> void m_ReadText(Stream& s, std::vector<T>* memory) {
		
			// Read in the size of the vector
		}

		Stream m_Stream;
		int m_Flags;

	public:
		Serialiser(int flags);

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
				// If writing text, wrap streamable's data under name of object
				if (!(m_Flags & Stream::Flags::BINARY)) {
					// NOTE: might make parsing the file more difficult later
					*m_Stream.out << m_Stream.GetTab() << "{\n";

					m_Stream.IncrementScope();

					object.Write(*this);

					m_Stream.DecrementScope();
					*m_Stream.out << m_Stream.GetTab() << "};\n";
				}
				else {
					object.Write(*this);
				}
			}
			// Its a trivial type
			else {
				if (m_Flags & Stream::Flags::BINARY) {
					m_WriteBinary(m_Stream, object);
				}
				else {
					//m_WriteText(m_Stream, object);
				}
			}
		}

		template <typename T>
		void Write(const std::vector<T>& object) {
			if constexpr (!IsStreamable<T>) {
				LogError("Object type {} is not Streamable", typeid(T).name());
			}
			// If the object inherits IStreamable, call IStreamable's write
			else if constexpr (std::is_base_of_v<IStreamable, T>) {
				if (m_Flags & Stream::Flag::BINARY) {
					unsigned int size = object.size();
					Write<unsigned int>(size);

					for (unsigned int i = 0; i < size; i++) {
						Write<T>(object[i]);
					}
				}
				else {
					m_WriteText(m_Stream, object);
				}
			}
			// Its a trivial type
			else {
				if (m_Flags & Stream::Flags::BINARY) {
					m_WriteBinary(m_Stream, object);
				}
				else {
					m_WriteText(m_Stream, object);
				}
			}
		}

		// NOTE: assuming the memory has been allocated for us
		template <typename T>
		void Read(T* memory) {
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
				if (m_Flags & Stream::Flags::BINARY) {
					m_ReadBinary(m_Stream, memory);
				}
				else {
					m_ReadText(m_Stream, memory);
				}
			}
		}

		template <typename T>
		void Read(std::vector<T>* memory) {
			// Ensure object is streamable
			if constexpr (!IsStreamable<T>) {
				LogError("Object type {} is not Streamable", typeid(T).name());
			}
			// If the object inherits IStreamable, call IStreamable's read
			else if constexpr (std::is_base_of_v<IStreamable, T>) {
				unsigned int size;
				Read<unsigned int>(&size);

				memory->resize(size);

				for (unsigned int i = 0; i < size; i++) {
					Read<T>(&memory->at(i));
				}
			}
			// Its a trivial type
			else {
				if (m_Flags & Stream::Flags::BINARY) {
					m_ReadBinary(m_Stream, memory);
				}
				else {
					m_ReadText(m_Stream, memory);
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