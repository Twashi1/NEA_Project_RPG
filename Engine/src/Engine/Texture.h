#pragma once

#include "Renderer.h"
#include "Utils.h"
#include <iostream>
#include <array>
#include <vector>
#include <memory>

#include "stb_image.h"

struct ENGINE_API Texture {
	static std::string PATH;

	GLuint id;
	
	unsigned int width;					// Width of texture
	unsigned int height;				// Height of texture
	std::shared_ptr<uint8_t[]> buffer;	// Buffer of colour data for texture

	static void Unbind();
	void Bind(uint8_t slot) const;

	void Create();
	void Delete();
	void Update(); // Deletes, then Creates texture to update it with new buffer data

	// Construct from colour data (3 bytes per pixel), and width/height of image
	Texture(std::shared_ptr<uint8_t[]> buffer, unsigned int width, unsigned int height);
	// Construct from png
	Texture(std::string filename);
	~Texture();
};