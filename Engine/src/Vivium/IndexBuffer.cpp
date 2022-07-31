#include "IndexBuffer.h"

namespace Vivium {
	IndexBuffer::IndexBuffer(const std::vector<uint16_t>& indices)
		: id(0), count(indices.size()), type(GL_UNSIGNED_SHORT)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint16_t), &indices[0], GL_STATIC_DRAW);

		Bind();
	}

	IndexBuffer::IndexBuffer(const std::vector<uint8_t>& indices)
		: id(0), count(indices.size()), type(GL_UNSIGNED_BYTE)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint8_t), &indices[0], GL_STATIC_DRAW);

		Bind();
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &id);
	}

	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}
}
