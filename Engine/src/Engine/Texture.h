#pragma once

#include "Renderer.h"
#include "Utilities.h"
#include <iostream>
#include <array>
#include <vector>
#include <memory>

struct ENGINE_API Texture {
	GLuint id;
	
	unsigned int width;					// Width of texture
	unsigned int height;				// Height of texture
	std::shared_ptr<uint8_t[]> buffer;	// Buffer of colour data for texture

	static void Unbind();
	void Bind(uint8_t slot) const;

	void Create();
	void Delete();

	Texture(std::shared_ptr<uint8_t[]> buffer, unsigned int width, unsigned int height);
	~Texture();
};