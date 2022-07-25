#pragma once

#include "Renderer.h"

namespace CoolEngineName {
	struct ENGINE_API IndexBuffer {
		GLuint id;
		GLushort count;
		GLenum type;

		IndexBuffer(const std::vector<unsigned short>& indices);
		~IndexBuffer();

		static void Unbind();
		void Bind() const;
	};
}
