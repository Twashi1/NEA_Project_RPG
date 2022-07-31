#pragma once

#include "Core.h"
#include "Logger.h"

namespace Vivium {
	namespace Serialiser {
		class VIVIUM_API Stream {
		public:
			std::ofstream* out;
			std::ifstream* in;

			// Open the file we're deserialising from
			void BeginRead(const char* path);
			// Close the file we're serialising to
			void BeginWrite(const char* path);
			// Close the file we're deserialising from
			void EndRead();
			// Close the file we're serialising to
			void EndWrite();

			Stream();
			~Stream();
		};

		class VIVIUM_API Streamable {
		public:
			Streamable();
			~Streamable();

			void Write(Stream& s) const;
			void Read(Stream& s);
		};

		// Serialises one instance of object
		template <typename T> VIVIUM_API extern void Serialise(Stream& s, const T& data);
		template <> VIVIUM_API void Serialise<::std::string>(Stream& s, const ::std::string& data);

		// Unserialises and returns one instance of type
		template <typename T> VIVIUM_API extern T Unserialise(Stream& s);
		template <> VIVIUM_API ::std::string Unserialise<::std::string>(Stream& s);

		// Unserialise one instance of object type T and store in "memory" (assuming memory has already been allocated)
		template <typename T> VIVIUM_API extern void Unserialise(Stream& s, T* memory);
		template <> VIVIUM_API void Unserialise<::std::string>(Stream& s, ::std::string* memory);
	}
}
