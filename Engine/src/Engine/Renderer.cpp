#include "Renderer.h"
#include "Renderable.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Font.h"
#include "Camera.h"
#include "TextRenderable.h"

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
Renderer::RenderTextMap_t Renderer::m_TextRenderables = {};
Camera* Renderer::camera = nullptr;
std::queue<uint8_t> Renderer::available_slots = std::queue<uint8_t>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });

uint8_t Renderer::GetTextureSlot()
{
	if (available_slots.size() > 1) {
		// Get front of queue and store it
		uint8_t id = available_slots.front();
		// Remove that id from list
		available_slots.pop();
		// Return that id
		return id;
	}

	Log("Ran out of texture slots (MAX 16 concurrent)", Utils::ERROR::WARNING);
	return Renderer::m_INVALID_TEXTURE_SLOT;
}

void Renderer::FreeTextureSlot(uint8_t slot)
{
	if (slot != Renderer::m_INVALID_TEXTURE_SLOT) {
		available_slots.push(slot);
	}
}

void Renderer::Update()
{
	// NOTE: arrangement of code will mean that text will ALWAYS be displays above a normal renderable even if the z level is lower
	//		 although this isn't that big of a problem since there are few cases where text needs to be drawn behind a renderable
	for (auto& [z, renderables] : m_Renderables) {
		for (Renderable* renderable : renderables) {
			Draw(renderable);
		}
	}

	for (auto& [z, renderables] : m_TextRenderables) {
		for (TextRenderable* renderable : renderables) {
			Draw(renderable);
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

void Renderer::Draw(const Renderable* renderable)
{
	// If renderable is visible
	if (renderable->isVisible) {
		
		Quad* quad = renderable->quad.get();

		// If its an object in the game world
		if (renderable->isGameWorld) {
			// Transform quad into screen coordinates
			quad = new Quad (camera->Transform(*renderable->quad));
		}

		// Get components
		const VertexBuffer& vb = quad->GetVertexBuffer();
		const IndexBuffer& ib = quad->GetIndexBuffer();

		// Bind components
		vb.Bind();
		ib.Bind();
		renderable->shader->Bind();

		// If the renderable has a texture, bind it as well
		uint8_t slot = Renderer::m_INVALID_TEXTURE_SLOT;
		if (renderable->texture != nullptr) {
			slot = Renderer::GetTextureSlot();
			renderable->texture->Bind(slot);
			renderable->shader->SetUniform1i("u_Texture", slot);
		}

		// Draw to screen
		GlCall(glDrawElements(GL_TRIANGLES, ib.count, ib.type, nullptr));

		// Unbind and delete the texture, and unbind texture slot
		Texture::Unbind();
		Renderer::FreeTextureSlot(slot); // NOTE: Function ignores m_INVALID_TEXTURE_SLOT

		// If its an object in the game world
		if (renderable->isGameWorld) {
			// Delete the transformed quad we made on heap
			delete quad;
		}
	}
}

void Renderer::Draw(const TextRenderable* renderable)
{
	uint8_t slot = Renderer::GetTextureSlot(); // Get slot we're drawing texture to

	// Ensure shader is not nullptr
	if (TextRenderable::shader == nullptr) {
		Log("Couldnt render text renderable since shader is not set", Utils::ERROR::WARNING);
	}

	// Bind shader and set texture slot
	TextRenderable::shader->Bind();
	TextRenderable::shader->SetUniform1i("u_Texture", slot);

	// Bind texture to that slot
	GlCall(glActiveTexture(GL_TEXTURE0 + slot));
	GlCall(glBindVertexArray(renderable->font->vertex_array_id));

	// Copy position to rendering_pos (so we can increment it to write subsequent characters)
	Vector2<float> rendering_pos = renderable->pos;

	// Iterate through characters in text
	for (char c : renderable->text) {
		const Font::Character& ch = renderable->font->character_map.at(c);

		float x = rendering_pos.x + ch.bearing.x * renderable->scale;
		float y = rendering_pos.y - (ch.size.y - ch.bearing.y) * renderable->scale;
		float w = ch.size.x * renderable->scale;
		float h = ch.size.y * renderable->scale;

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
		GlCall(glBindBuffer(GL_ARRAY_BUFFER, renderable->font->vertex_buffer_id));
		GlCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

		// Unbind buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		GlCall(glDrawArrays(GL_TRIANGLES, 0, 6));
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		rendering_pos.x += (ch.advance >> 6) * renderable->scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	// Unbind vertex array and texture
	GlCall(glBindVertexArray(0));
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	// Make texture slot available again
	Renderer::FreeTextureSlot(slot);
}

void Renderer::DrawText(std::string text, Vector2<float> pos, float scale, Vector3<float> color, Shader& shader, const Font& font)
{
	uint8_t slot = Renderer::GetTextureSlot(); // Get slot we're drawing texture to

	shader.Bind();
	shader.SetUniform1i("u_Texture", slot);
	shader.SetUniform3f("u_TextColor", color);
	
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
