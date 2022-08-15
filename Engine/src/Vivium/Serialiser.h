#pragma once

#include "Core.h"
#include "Logger.h"
#include "Vector2.h"
#include "Utils.h"

/*
TODO: add functionality for pointers?
TODO: text serialisation
TODO: containers
TODO: cleanup
TODO: split IStreamable, Serialiser, Stream into seperate files
TODO: map/unordered map
*/

namespace Vivium {
	class IStreamable;
	// TODO still ugly
	template <typename T> inline constexpr bool __BaseStreamableTypes = std::is_arithmetic_v<T> || std::is_same_v<T, std::string> || std::is_same_v<T, bool>;
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
	public:
		enum Flags : int {
			TEXT = NULL,
			BINARY = std::ios::binary,
			TRUNC = std::ios::trunc,
			APPEND = std::ios::app
		};

		std::ofstream* out = nullptr;
		std::ifstream* in = nullptr;

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
		void m_WriteBinary(Stream& s, const T* data, const unsigned int& length) {
			static_assert(!std::is_same_v<T, std::string>, "Cannot write array of strings, use std::vector instead");

			s.out->write((char*)data, sizeof(T) * length);
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

			std::memcpy(memory, readbuff, sizeof(T)); // Copy data from buffer into pointer given

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
			std::memcpy(&memory->at(0), readbuff, sizeof(T) * size);

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
		void m_ReadBinary(Stream& s, T* memory, const unsigned int& length) {
			static_assert(!std::is_same_v<T, std::string>, "Cannot read array of strings, use std::vector instead");

			// Assuming memory has already been allocated
			char* readbuff = new char[sizeof(T) * length];
			s.in->read(readbuff, sizeof(T) * length);

			std::memcpy(memory, readbuff, sizeof(T) * length);

			delete[] readbuff;
		}

		template <typename T>
		void m_ReadBinary(Stream& s, Vector2<T>* memory) {
			char* readbuff = new char[sizeof(Vector2<T>)];
			s.in->read(readbuff, sizeof(Vector2<T>));

			std::memcpy(memory, readbuff, sizeof(Vector2<T>));

			delete[] readbuff;
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
				object.Write(*this);
			}
			// Its a trivial type
			else {
				m_WriteBinary(m_Stream, object);
			}
		}

		template <typename T>
		void Write(const std::vector<T>& object) {
			if constexpr (!IsStreamable<T>) {
				LogError("Object type {} is not Streamable", typeid(T).name());
			}
			// If the object inherits IStreamable, call IStreamable's write
			else if constexpr (std::is_base_of_v<IStreamable, T>) {
				unsigned int size = object.size();
				Write<unsigned int>(size);

				for (unsigned int i = 0; i < size; i++) {
					Write<T>(object[i]);
				}
			}
			// Its a trivial type
			else {
				m_WriteBinary(m_Stream, object);
			}
		}

		template <typename T, std::size_t Size>
		void Write(const std::array<T, Size>& object) {
			if constexpr (!IsStreamable<T>) {
				LogError("Object type {} is not Streamable", typeid(T).name());
			}
			// If the object inherits IStreamable, call IStreamable's write
			else if constexpr (std::is_base_of_v<IStreamable, T>) {
				Write<unsigned int>(Size);

				for (unsigned int i = 0; i < Size; i++) {
					Write<T>(object[i]);
				}
			}
			// Its a trivial type
			else {
				m_WriteBinary(m_Stream, object);
			}
		}

		template <typename T>
		void Write(const T* object_array, const unsigned int& length) {
			if constexpr (!IsStreamable<T>) {
				LogError("Object type {} is not Streamable", typeid(T).name());
			}
			// If the object inherits IStreamable, call IStreamable's write
			else if constexpr (std::is_base_of_v<IStreamable, T>) {
				for (unsigned int i = 0; i < length; i++) {
					Write<T>(object_array[i]);
				}
			}
			// Its a trivial type
			else {
				m_WriteBinary(m_Stream, object_array, length);
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
				m_ReadBinary(m_Stream, memory);
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
				m_ReadBinary(m_Stream, memory);
			}
		}

		template <typename T, std::size_t Size>
		void Read(std::array<T, Size>* memory) {
			// Ensure object is streamable
			if constexpr (!IsStreamable<T>) {
				LogError("Object type {} is not Streamable", typeid(T).name());
			}
			// If the object inherits IStreamable, call IStreamable's read
			else if constexpr (std::is_base_of_v<IStreamable, T>) {
				for (unsigned int i = 0; i < Size; i++) {
					Read<T>(&memory->at(i));
				}
			}
			// Its a trivial type
			else {
				m_ReadBinary(m_Stream, memory);
			}
		}

		template <typename T>
		void Read(T* memory, const unsigned int& length) {
			// Ensure object is streamable
			if constexpr (!IsStreamable<T>) {
				LogError("Object type {} is not Streamable", typeid(T).name());
			}
			// If the object inherits IStreamable, call IStreamable's read
			else if constexpr (std::is_base_of_v<IStreamable, T>) {
				for (unsigned int i = 0; i < length; i++) {
					Read<T>(&memory[i]);
				}
			}
			// Its a trivial type
			else {
				m_ReadBinary(m_Stream, memory, length);
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