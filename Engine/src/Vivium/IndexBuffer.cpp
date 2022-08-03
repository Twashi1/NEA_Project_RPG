#include "IndexBuffer.h"

namespace Vivium {
	IndexBuffer::IndexBuffer(const std::vector<uint16_t>& indices)
		: m_ID(0), m_Count(indices.size()), m_GLType(GL_UNSIGNED_SHORT)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint16_t), &indices[0], GL_STATIC_DRAW);

		Bind();
	}

	IndexBuffer::IndexBuffer(const std::vector<uint8_t>& indices)
		: m_ID(0), m_Count(indices.size()), m_GLType(GL_UNSIGNED_BYTE)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint8_t), &indices[0], GL_STATIC_DRAW);

		Bind();
	}

	IndexBuffer::IndexBuffer(const void* data, const std::size_t& size, const std::size_t& count, GLenum gl_type)
		: m_ID(0), m_Count(count), m_GLType(gl_type)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		Bind();
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	const GLuint& IndexBuffer::GetID() const { return m_ID; }

	const GLushort& IndexBuffer::GetCount() const { return m_Count; }

	const GLenum& IndexBuffer::GetType() const { return m_GLType; }

	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}
}
