#include "IndexBuffer.h"

namespace Vivium {
	IndexBuffer::IndexBuffer(const std::vector<unsigned short>& indices)
		: id(0)
	{
		count = indices.size();
		type = GL_UNSIGNED_SHORT;

		GlCall(glGenBuffers(1, &id));
		GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
		GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW));

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
		GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
	}
}