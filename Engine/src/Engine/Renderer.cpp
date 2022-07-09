#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Font.h"
#include "Camera.h"
#include "Text.h"
#include "Animation.h"
#include "Quad.h"
#include "Button.h"

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

std::queue<uint8_t> Renderer::available_slots = std::queue<uint8_t>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
Shader* Renderer::texture_shader = nullptr;
Camera* Renderer::camera = nullptr;

ENGINE_API uint8_t Renderer::GetTextureSlot()
{
	if (available_slots.size() > 1) {
		// Get front of queue and store it
		uint8_t id = available_slots.front();
		// Remove that id from list
		available_slots.pop();
		// Return that id
		return id;
	}

	Log("Ran out of texture slots (MAX 16 concurrent)", LOG::WARNING);
	return Renderer::m_INVALID_TEXTURE_SLOT;
}

ENGINE_API void Renderer::FreeTextureSlot(uint8_t slot)
{
	if (slot != Renderer::m_INVALID_TEXTURE_SLOT) {
		available_slots.push(slot);
	}
}

ENGINE_API void Renderer::BeginFrame()
{
	// DEPRECATED?
}

ENGINE_API void Renderer::EndFrame()
{
	// DEPRECATED?
}

ENGINE_API void Renderer::Schedule(const Quad* quad, Shader* shader, float z)
{
	quad->GetVertexBuffer().Bind();
	const IndexBuffer& ib = quad->GetIndexBuffer(); ib.Bind();
	shader->Bind();
	shader->SetUniform1f("u_ZCoord", z);

	GlCall(glDrawElements(GL_TRIANGLES, ib.count, ib.type, nullptr));
}

ENGINE_API void Renderer::Schedule(const Quad* quad, const Texture* texture, float z)
{
	quad->GetVertexBuffer().Bind();
	const IndexBuffer& ib = quad->GetIndexBuffer(); ib.Bind();

	// Bind texture
	uint8_t slot = GetTextureSlot();
	texture->Bind(slot);

	// Bind shader
	Renderer::texture_shader->Bind();
	Renderer::texture_shader->SetUniform1f("u_ZCoord", z);
	Renderer::texture_shader->SetUniform1i("u_Texture", slot);

	GlCall(glDrawElements(GL_TRIANGLES, ib.count, ib.type, nullptr));

	// Unbind texture
	Texture::Unbind();
	FreeTextureSlot(slot);
}

ENGINE_API void Renderer::Schedule(const Quad* quad, Shader* shader, const Texture* texture, float z)
{
	quad->GetVertexBuffer().Bind();
	const IndexBuffer& ib = quad->GetIndexBuffer(); ib.Bind();

	// Bind texture
	uint8_t slot = GetTextureSlot();
	texture->Bind(slot);

	// Bind shader
	shader->Bind();
	shader->SetUniform1f("u_ZCoord", z);
	shader->SetUniform1i("u_Texture", slot);

	GlCall(glDrawElements(GL_TRIANGLES, ib.count, ib.type, nullptr));

	// Unbind texture
	Texture::Unbind();
	FreeTextureSlot(slot);
}

ENGINE_API void Renderer::Schedule(const Text* text, float z)
{
	uint8_t slot = Renderer::GetTextureSlot(); // Get slot we're drawing texture to

	// Bind shader and set texture slot
	text->shader->Bind();
	text->shader->SetUniform1i("u_Texture", slot);
	text->shader->SetUniform1f("u_ZCoord", z);

	// Bind texture to that slot
	GlCall(glActiveTexture(GL_TEXTURE0 + slot));
	GlCall(glBindVertexArray(text->font->vertex_array_id));

	// Copy position to rendering_pos (so we can increment it to write subsequent characters)
	Vector2<float> rendering_pos = text->pos;

	// Iterate through characters in text
	for (char c : text->text) {
		const Font::Character& ch = text->font->character_map.at(c);

		float x = rendering_pos.x + ch.bearing.x * text->scale;
		float y = rendering_pos.y - (ch.size.y - ch.bearing.y) * text->scale;
		float w = ch.size.x * text->scale;
		float h = ch.size.y * text->scale;

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
		GlCall(glBindBuffer(GL_ARRAY_BUFFER, text->font->vertex_buffer_id));
		GlCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

		// Unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		GlCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		rendering_pos.x += (ch.advance >> 6) * text->scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	// Unbind vertex array and texture
	GlCall(glBindVertexArray(0));
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	// Make texture slot available again
	Renderer::FreeTextureSlot(slot);
}

ENGINE_API void Renderer::Schedule(Button* btn, float z)
{
	Renderer::Schedule(btn->text, z + 0.1);
	
	const Texture* current_texture = btn->CurrentTexture();
	if (current_texture != nullptr) {
		Renderer::Schedule(&btn->quad, btn->CurrentShader(), current_texture, z);
	}
	else {
		Renderer::Schedule(&btn->quad, btn->CurrentShader(), z);
	}
	
}

ENGINE_API void Renderer::Schedule(Animation* animation, float z)
{
	Renderer::Schedule(animation->quad.get(), animation->shader.get(), animation->GetAtlas().get(), z);
}

ENGINE_API void Renderer::m_Draw(const VertexBuffer& vb, const IndexBuffer& ib, const Shader& shader)
{
	// Bind all the components we need
	vb.Bind();
	ib.Bind();
	shader.Bind();

	GlCall(glDrawElements(GL_TRIANGLES, ib.count, ib.type, nullptr));
}
