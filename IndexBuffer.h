#pragma once

#include <vector>
#include "Renderer.h"

struct IndexBuffer {
	GLuint id;
	GLushort count;
	GLenum type;

	IndexBuffer(const std::vector<unsigned short>& indices);
	~IndexBuffer();

	static void Unbind();
	void Bind() const;
};