#pragma once

#include "Core.h"
#include "Logger.h"

// https://www.youtube.com/watch?v=jIJFM_pi6gQ&ab_channel=TheCherno

namespace Vivium {
	class VIVIUM_API GLSLDataType {
	private:
		GLSLDataType(unsigned int size, unsigned int gl_type, unsigned int component_count);

	public:
		GLSLDataType(const GLSLDataType& other);
		// TODO probably never gonna be used but ill make it just in case
		GLSLDataType(GLSLDataType&& other) noexcept;

		GLSLDataType& operator=(const GLSLDataType& other)
		{
			size = other.size;
			gl_type = other.gl_type;
			component_count = other.component_count;

			return *this;
		}

		// TODO make private with getter functions
		unsigned int size;
		unsigned int gl_type;
		unsigned int component_count;

		static const GLSLDataType FLOAT;
		static const GLSLDataType VEC2;
		static const GLSLDataType VEC3;
		static const GLSLDataType VEC4;
		static const GLSLDataType INT;
		static const GLSLDataType IVEC2;
		static const GLSLDataType IVEC3;
		static const GLSLDataType IVEC4;
		static const GLSLDataType UINT;
		static const GLSLDataType UVEC2;
		static const GLSLDataType UVEC3;
		static const GLSLDataType UVEC4;
		static const GLSLDataType BOOL;
	};

	class VIVIUM_API BufferElement {
	public:
		std::string name;
		GLSLDataType type;
		unsigned int offset;

		BufferElement() = default;
		BufferElement(const std::string& name, const GLSLDataType& type);
		BufferElement(const BufferElement& other);

		BufferElement& operator=(const BufferElement& other)
		{
			name = other.name;
			type = other.type;
			offset = other.offset;

			return *this;
		}
	};

	class VIVIUM_API BufferLayout {
	private:
		std::vector<BufferElement> m_Elements;
		unsigned int m_Stride = 0;

		// Calculates offsets and stride
		void m_Calculate();

	public:
		// TODO: iterators?

		const std::vector<BufferElement>& GetElements() const;
		unsigned int GetStride() const;

		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements);
	};

	class VIVIUM_API VertexBuffer {
	private:
		GLuint m_ID;
		BufferLayout m_Layout;

	public:
		VertexBuffer(const std::vector<float>& data, const BufferLayout& layout);
		VertexBuffer(const void* data, const std::size_t& size, const BufferLayout& layout);
		~VertexBuffer();

		static void Unbind();
		void Bind() const;

		const BufferLayout& GetLayout() const;
		const GLuint& GetID() const;

		void Set(const std::vector<float>& vertices);
		void Set(const void* data, const std::size_t& size);
		void Set(unsigned int element_index, const void* data, const std::size_t& size);

		void* StartMap();
		void EndMap();
	};
}
