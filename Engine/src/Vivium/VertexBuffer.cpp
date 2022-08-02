#include "VertexBuffer.h"

namespace Vivium {
	const GLSLDataType GLSLDataType::FLOAT = { sizeof(float), GL_FLOAT, 1 };
	const GLSLDataType GLSLDataType::VEC2 = { sizeof(float) * 2, GL_FLOAT, 2 };
	const GLSLDataType GLSLDataType::VEC3 = { sizeof(float) * 3, GL_FLOAT, 3 };
	const GLSLDataType GLSLDataType::VEC4 = { sizeof(float) * 4, GL_FLOAT, 4 };
	const GLSLDataType GLSLDataType::INT = { sizeof(int), GL_INT, 1 };
	const GLSLDataType GLSLDataType::IVEC2 = { sizeof(int) * 2, GL_INT, 2 };
	const GLSLDataType GLSLDataType::IVEC3 = { sizeof(int) * 3, GL_INT, 3 };
	const GLSLDataType GLSLDataType::IVEC4 = { sizeof(int) * 4, GL_INT, 4 };
	const GLSLDataType GLSLDataType::UINT = { sizeof(unsigned int), GL_UNSIGNED_INT, 1 };
	const GLSLDataType GLSLDataType::UVEC2 = { sizeof(unsigned int) * 2, GL_UNSIGNED_INT, 2 };
	const GLSLDataType GLSLDataType::UVEC3 = { sizeof(unsigned int) * 3, GL_UNSIGNED_INT, 3 };
	const GLSLDataType GLSLDataType::UVEC4 = { sizeof(unsigned int) * 4, GL_UNSIGNED_INT, 4 };
	const GLSLDataType GLSLDataType::BOOL = { sizeof(bool), GL_BOOL, 1 };

	GLSLDataType::GLSLDataType(unsigned int size, unsigned int type, unsigned int component_count)
		: size(size), gl_type(type), component_count(component_count)
	{}

	GLSLDataType::GLSLDataType(const GLSLDataType & other)
		: size(other.size), gl_type(other.gl_type), component_count(other.component_count)
	{}

	GLSLDataType::GLSLDataType(GLSLDataType&& other) noexcept
		: size(std::move(other.size)), gl_type(std::move(other.gl_type)), component_count(std::move(other.component_count))
	{}

	VertexBuffer::VertexBuffer(const std::vector<float>& data, const BufferLayout& layout)
		: id(0), layout(layout)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

		const std::vector<BufferElement>& elements = layout.GetElements();

		for (int i = 0; i < elements.size(); i++) {
			const BufferElement& element = elements[i];

			glEnableVertexAttribArray(i);
			glVertexAttribPointer(
				i,								// Layout location
				element.type.component_count,	// Amount of components of type
				element.type.gl_type,			// GL base type
				GL_FALSE,						// Is normalised
				layout.GetStride(),				// Stride
				(const void*)element.offset		// Offset
			);
		}
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &id);
	}

	void VertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
		const std::vector<BufferElement>& elements = layout.GetElements();

		for (int i = 0; i < elements.size(); i++) {
			const BufferElement& element = elements[i];

			glEnableVertexAttribArray(i);
			glVertexAttribPointer(
				i,								// Layout location
				element.type.component_count,	// Amount of components of type
				element.type.gl_type,			// GL base type
				GL_FALSE,						// Is normalised
				layout.GetStride(),				// Stride
				(const void*)element.offset		// Offset
			);
		}
	}

	void VertexBuffer::Set(const std::vector<float>& data)
	{
		Bind();
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	}
	
	BufferElement::BufferElement(const std::string& name, const GLSLDataType& type)
		: name(name), type(type), offset(0) {}

	BufferElement::BufferElement(const BufferElement& other)
		: name(other.name), type(other.type), offset(other.offset)
	{}

	// Calculates offsets and stride

	void BufferLayout::m_Calculate() {
		unsigned int running_offset = 0;

		for (BufferElement& element : m_Elements) {
			element.offset = running_offset;
			running_offset += element.type.size;
			m_Stride += element.type.size;
		}
	}

	const std::vector<BufferElement>& BufferLayout::GetElements() const { return m_Elements; }

	unsigned int BufferLayout::GetStride() const { return m_Stride; }

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
	{
		m_Calculate();
	}
}
