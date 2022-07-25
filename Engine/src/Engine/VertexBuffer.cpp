#include "VertexBuffer.h"

namespace CoolEngineName {
	VertexBuffer::VertexBuffer(const std::vector<float>& vertices)
		: ids({ 0 })
	{
		glGenBuffers(1, &ids[0]);
		GlCall(glBindBuffer(GL_ARRAY_BUFFER, ids[0]));
		GlCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW));
		GlCall(glEnableVertexAttribArray(0));
		GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
	}

	VertexBuffer::VertexBuffer(const std::vector<float>& vertices, const std::vector<float>& tex_coords)
		: ids({ 0, 0 })
	{
		glGenBuffers(2, &ids[0]);

		GlCall(glBindBuffer(GL_ARRAY_BUFFER, ids[0]));
		GlCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW));
		GlCall(glEnableVertexAttribArray(0));
		GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

		GlCall(glBindBuffer(GL_ARRAY_BUFFER, ids[1]));
		GlCall(glBufferData(GL_ARRAY_BUFFER, tex_coords.size() * sizeof(float), &tex_coords[0], GL_STATIC_DRAW));
		GlCall(glEnableVertexAttribArray(1));
		GlCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
	}

	VertexBuffer::~VertexBuffer()
	{
		GlCall(glDeleteBuffers(ids.size(), &ids[0]));
	}

	void VertexBuffer::Unbind()
	{
		GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void VertexBuffer::Bind() const
	{
		if (ids.size() > 1) {
			GlCall(glBindBuffer(GL_ARRAY_BUFFER, ids[1]));
			GlCall(glEnableVertexAttribArray(1));
			GlCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
		}

		GlCall(glBindBuffer(GL_ARRAY_BUFFER, ids[0]));
		GlCall(glEnableVertexAttribArray(0));
		GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
	}

	void VertexBuffer::Set(const std::vector<float>& vertices)
	{
		GlCall(glBindBuffer(GL_ARRAY_BUFFER, ids[0]));
		GlCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW));
	}

	void VertexBuffer::SetTex(const std::vector<float>& tex_coords)
	{
		if (ids.size() > 1) {
			GlCall(glBindBuffer(GL_ARRAY_BUFFER, ids[1]));
			GlCall(glBufferData(GL_ARRAY_BUFFER, tex_coords.size() * sizeof(float), &tex_coords[0], GL_STATIC_DRAW));
			GlCall(glEnableVertexAttribArray(1));
			GlCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
		}
		else {
			ENG_LogWarn("Attempting to set texture coordinates for VertexBuffer that has no texture coordinates");
		}
	}
}