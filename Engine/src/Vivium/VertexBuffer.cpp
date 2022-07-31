#include "VertexBuffer.h"

namespace Vivium {
	VertexBuffer::VertexBuffer(const std::vector<float>& data, unsigned int layout_index)
		: id(0), layout(layout_index)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(layout_index);
		// layout index, 2 values per vertex, floats, not normalised, 16 bytes per vertex
		glVertexAttribPointer(layout_index, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
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
		glEnableVertexAttribArray(layout);
		glVertexAttribPointer(layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	}

	void VertexBuffer::Set(const std::vector<float>& vertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	}
}
