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
struct Camera;
class Text;
class Button;
class TextInput;
class Animation;
struct Quad;
struct Texture;

class ENGINE_API Renderer {
private:
	static constexpr uint8_t m_INVALID_TEXTURE_SLOT = 255; // Constant to represent an invalid texture slot

public:
	static Camera* camera; // Camera currently being used on all Renderable objects
	static Shader* texture_shader; // Default shader used for textures
	static std::vector<uint8_t> available_slots; // Available slots on GPU for texture

	static void Init(Camera* camera);

	// Gives a free texture slot
	static uint8_t GetTextureSlot();
	// Makes a texture slot available again
	static void FreeTextureSlot(uint8_t slot);

	static void Schedule(const Quad* quad, Shader* shader);
	static void Schedule(const Quad* quad, const Texture* texture);
	static void Schedule(const Quad* quad, const Texture* texture, uint8_t slot);
	static void Schedule(const Quad* quad, Shader* shader, const Texture* texture);
	static void Schedule(const Text* text);
	static void Schedule(Button* btn);
	static void Schedule(Animation* animation);
	static void Schedule(TextInput* text_input);
};