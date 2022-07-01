#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <string>
#include <queue>
#include <map>

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
struct Renderable;
struct Camera;
struct TextRenderable;

// TODO: change api

class ENGINE_API Renderer {
private:
	// Shorthands for RenderMap types
	typedef std::map<int, Utils::List<Renderable*>> RenderMap_t;
	typedef std::map<int, Utils::List<TextRenderable*>> RenderTextMap_t;

	static RenderMap_t m_Renderables;					   // Stores all text renderable objects, sorting them by their z level
	static RenderTextMap_t m_TextRenderables;			   // Stores all renderable objects, sorting them by their z level
	static constexpr uint8_t m_INVALID_TEXTURE_SLOT = 255; // Constant to represent an invalid texture slot

public:
	static Camera* camera; // Camera currently being used on all Renderable objects
	static std::queue<uint8_t> available_slots; // Available slots on GPU for texture

	// Gives a free texture slot
	static uint8_t GetTextureSlot();
	// Makes a texture slot available again
	static void FreeTextureSlot(uint8_t slot);

	// Draws all objects in m_Renderables and m_TextRenderables
	static void Update();

	// Draw shape to screen given a vertex buffer, index buffer, and shader
	static void Draw(const VertexBuffer& vb, const IndexBuffer& ib, const Shader& shader);
	// Forcibly draw renderable (ignores z level)
	static void Draw(const Renderable* renderable);
	// Draw text renderable (ignores z level)
	static void Draw(const TextRenderable* renderable);
	// Draw text to screen
	static void DrawText(std::string text, Vector2<float> pos, float scale, Vector3<float> color, Shader& shader, const Font& font);
	// Clear the screen
	static void Clear();

	friend Renderable;
	friend TextRenderable;
};