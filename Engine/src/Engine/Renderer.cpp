#include "Renderer.h"
#include "Renderable.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Font.h"
#include "Camera.h"

void GlClearError() {
	// While we have an error stay in function
	while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error @ " << function << "]: {" << error << ": " << glewGetErrorString(error) << "} " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

Renderer::RenderMap_t Renderer::m_Renderables = {};
Camera* Renderer::camera = nullptr;
std::queue<uint8_t> Renderer::available_slots = std::queue<uint8_t>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });

uint8_t Renderer::GetTextureSlot()
{
	if (available_slots.size() > 1) {
		uint8_t id = available_slots.front();
		available_slots.pop();
		return id;
	}

	Log("Ran out of texture slots", Utils::ERROR::WARNING);
	return 0;
}

void Renderer::FreeTextureSlot(uint8_t slot)
{
	available_slots.push(slot);
}

void Renderer::Update()
{
	// Because declared as std::map, key values should be in ascending order
	// Iterate over key value pairs
	for (auto& [z, renderables] : m_Renderables) {
		// Iterate over Renderable* for each z level
		for (Renderable* renderable : renderables) {
			// Transform quad into screen coordinates
			Quad screen_quad = camera->Transform(*renderable->quad);
			
			// Get components
			const VertexBuffer& vb = screen_quad.GetVertexBuffer();
			const IndexBuffer& ib = screen_quad.GetIndexBuffer();

			// Bind components
			vb.Bind();
			ib.Bind();
			renderable->shader->Bind();

			// Draw to screen
			GlCall(glDrawElements(GL_TRIANGLES, ib.count, ib.type, nullptr));
		}
	}
}

void Renderer::Draw(const VertexBuffer& vb, const IndexBuffer& ib, const Shader& shader)
{
	// Bind all the components we need
	vb.Bind();
	ib.Bind();
	shader.Bind();

	GlCall(glDrawElements(GL_TRIANGLES, ib.count, ib.type, nullptr));
}

void Renderer::DrawText(std::string text, Vector2<float> pos, float scale, Vector3<float> color, Shader& shader, const Font& font)
{
	uint8_t slot = Renderer::GetTextureSlot(); // Get slot we're drawing texture to

	shader.Bind();
	shader.SetUniform1i("u_Texture", slot);
	shader.SetUniform3f("u_TextColor", color);
	
	// TODO: maybe something is already on GPU at 0?
	// Bind texture
	GlCall(glActiveTexture(GL_TEXTURE0 + slot));
	GlCall(glBindVertexArray(font.vertex_array_id));
	// Tell shader which slot texture is in

	// Iterate through characters in text
	for (char c : text) {
		const Font::Character& ch = font.character_map.at(c);

		float x = pos.x + ch.bearing.x * scale;
		float y = pos.y - (ch.size.y - ch.bearing.y) * scale;
		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		// Update vertex buffer for each character
		float vertices[6][4] = {
			{ x,     y + h,   0.0f, 0.0f },
			{ x,     y,       0.0f, 1.0f },
			{ x + w, y,       1.0f, 1.0f },

			{ x,     y + h,   0.0f, 0.0f },
			{ x + w, y,       1.0f, 1.0f },
			{ x + w, y + h,   1.0f, 0.0f }
		};

		// Render glyph texture over quad
		GlCall(glBindTexture(GL_TEXTURE_2D, ch.texture_id));
		// Update content of vertex buffer memory
		GlCall(glBindBuffer(GL_ARRAY_BUFFER, font.vertex_buffer_id));
		GlCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

		// Unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		GlCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		pos.x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	// Unbind vertex array and texture
	GlCall(glBindVertexArray(0));
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));
	// Make texture slot available again
	Renderer::FreeTextureSlot(slot);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}
