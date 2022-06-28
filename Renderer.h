#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <string>
#include <queue>

#include "Vector2.h"
#include "Vector3.h"

#define GlCall(x) GlClearError();\
	x;\
	GlLogCall(#x, __FILE__, __LINE__)

void GlClearError();
bool GlLogCall(const char* function, const char* file, int line);

struct VertexBuffer;
struct IndexBuffer;
class Shader;
class Font;

class Renderer {
public:
	static std::queue<uint8_t> available_slots; // Available slots on GPU for texture

	// Gives a free texture slot
	static uint8_t GetTextureSlot();
	// Makes a texture slot available again
	static void FreeTextureSlot(uint8_t slot);

	// Draw shape to screen given a vertex buffer, index buffer, and shader
	static void Draw(const VertexBuffer& vb, const IndexBuffer& ib, const Shader& shader);
	// Draw text to screen
	static void DrawText(std::string text, Vector2<float> pos, float scale, Vector3<float> color, Shader& shader, const Font& font);
	// Clear the screen
	static void Clear();
};