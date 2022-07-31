#pragma once

#include "Core.h"
#include "Logger.h"

namespace Vivium {
	class VIVIUM_API VertexBuffer {
	public:
		GLuint id;
		GLuint layout;

		VertexBuffer(const std::vector<float>& data, unsigned int layout_index = 0);
		~VertexBuffer();

		static void Unbind();
		void Bind() const;

		void Set(const std::vector<float>& vertices);
	};
}
