#pragma once

#include "Core.h"
#include "Logger.h"

namespace Vivium {
	class VIVIUM_API IndexBuffer {
	public:
		GLuint id;
		GLushort count; // Amount of indices
		GLenum type; // Datatype of each index

		IndexBuffer(const std::vector<uint16_t>& indices);
		IndexBuffer(const std::vector<uint8_t>& indices);
		~IndexBuffer();

		static void Unbind();
		void Bind() const;
	};
}