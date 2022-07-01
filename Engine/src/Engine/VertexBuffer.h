#pragma once

#include <vector>
#include <iostream>
#include "Renderer.h"

struct ENGINE_API VertexBuffer {
	std::vector<GLuint> ids;

	VertexBuffer(const std::vector<float>& vertices);
	VertexBuffer(const std::vector<float>& vertices, const std::vector<float>& tex_coords);
	~VertexBuffer();

	static void Unbind();
	void Bind() const;

	// Sets vertices
	void Set(const std::vector<float>& vertices);
	// Sets texture coordinates
	void SetTex(const std::vector<float>& tex_coords);
};