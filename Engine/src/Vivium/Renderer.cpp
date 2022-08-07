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
#include "TextInput.h"
#include "Slider.h"
#include "Sprite.h"
#include "Application.h"

namespace Vivium {
	std::vector<uint8_t> Renderer::m_AvailableSlots = std::vector<uint8_t>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
	Ref(Camera) Renderer::camera = nullptr;

	std::map<int, Framebuffer*> Renderer::m_Framebuffers = {};
	Framebuffer* Renderer::m_CurrentScene = nullptr;
	Shader* Renderer::m_FramebufferShader = nullptr;

	void Renderer::m_Terminate()
	{
		Framebuffer::Unbind();

		for (auto [id, fb] : m_Framebuffers) {
			delete fb;
		}

		m_Framebuffers = {};

		delete m_FramebufferShader;
	}

	void Renderer::m_Init()
	{
		camera = MakeRef(Camera, 0.0f, (float)Application::width, 0.0f, (float)Application::height);

		m_FramebufferShader = new Shader("static_texture_vertex", "texture_frag");
	}

	void Renderer::m_ResizeFramebuffers(int width, int height)
	{
		for (auto [id, fb] : m_Framebuffers) {
			fb->Resize(width, height);
		}
	}

	void Renderer::SetTarget(const Framebuffer* fb)
	{
		if (fb != nullptr) { fb->Bind(); }
		else { Framebuffer::Unbind(); }
	}

	void Renderer::ResetTarget()
	{
		Framebuffer::Unbind();
	}

	void Renderer::BeginScene(int id)
	{
		auto it = m_Framebuffers.find(id);

		// If framebuffer already exists
		if (it != m_Framebuffers.end()) {
			it->second->Bind();
			m_CurrentScene = it->second;
		}
		// Framebuffer doesn't already exist, create it
		else {
			Vector2<int> screen_dim = Application::GetScreenDim();
			m_CurrentScene = new Framebuffer(screen_dim.x, screen_dim.y);
			m_Framebuffers.insert({id, m_CurrentScene});
			m_CurrentScene->Bind();
		}
	}

	void Renderer::EndScene()
	{
		Framebuffer::Unbind();
		m_CurrentScene = nullptr;
	}

	void Renderer::DrawScenes()
	{
		Vector2<float> screen_dim = (Vector2<float>)Application::GetScreenDim();

		Quad screen_quad = Quad(screen_dim * 0.5f, screen_dim);

		for (auto& [id, fb] : m_Framebuffers) {
			Renderer::Submit(&screen_quad, m_FramebufferShader, fb);
			fb->Clear();
		}
	}

	void Renderer::DrawScene(int scene_ID)
	{
		auto it = m_Framebuffers.find(scene_ID);

		if (it != m_Framebuffers.end()) {
			Vector2<float> screen_dim = (Vector2<float>)Application::GetScreenDim();

			Quad screen_quad = Quad(screen_dim * 0.5f, screen_dim);

			Renderer::Submit(&screen_quad, m_FramebufferShader, it->second);
			it->second->Clear();
		}
		else {
			LogError("Couldn't find scene with ID {}, ignoring call", scene_ID);
		}
	}
	
	uint8_t Renderer::GetTextureSlot()
	{
		if (m_AvailableSlots.size() > 1) {
			// Get front of queue and store it
			uint8_t id = m_AvailableSlots.back();
			// Remove that id from list
			m_AvailableSlots.pop_back();
			// Return that id
			return id;
		}

		LogInfo("Ran out of texture slots (MAX 16 concurrent)");
		return Renderer::INVALID_TEXTURE_SLOT;
	}

	void Renderer::FreeTextureSlot(uint8_t slot)
	{
		if (slot != Renderer::INVALID_TEXTURE_SLOT) {
			m_AvailableSlots.emplace_back(slot);
		}
	}

	void Renderer::Submit(const VertexBuffer* vb, const IndexBuffer* ib, Shader* shader, Texture* texture, uint8_t slot)
	{
		vb->Bind(); ib->Bind(); shader->Bind(); texture->Bind(slot);
		shader->SetUniformMat4fv("u_ProjMat", camera->GetProjMat());
		shader->SetUniformMat4fv("u_ViewMat", camera->GetViewMat());
		shader->SetUniform1f("u_Time", Timer::GetTime());
		shader->SetUniform1i("u_Texture", slot);

		glDrawElements(GL_TRIANGLES, ib->GetCount(), ib->GetType(), nullptr);
	}

	void Renderer::Submit(const VertexBuffer* vb, const IndexBuffer* ib, Shader* shader)
	{
		vb->Bind(); ib->Bind(); shader->Bind();
		shader->SetUniformMat4fv("u_ProjMat", camera->GetProjMat());
		shader->SetUniformMat4fv("u_ViewMat", camera->GetViewMat());
		shader->SetUniform1f("u_Time", Timer::GetTime());

		glDrawElements(GL_TRIANGLES, ib->GetCount(), ib->GetType(), nullptr);
	}

	void Renderer::Submit(const VertexBuffer* vb, const IndexBuffer* ib, Shader* shader, const Framebuffer* fb, uint8_t slot)
	{
		vb->Bind(); ib->Bind(); shader->Bind(); fb->SetSlot(slot);
		shader->SetUniformMat4fv("u_ProjMat", camera->GetProjMat());
		shader->SetUniformMat4fv("u_ViewMat", camera->GetViewMat());
		shader->SetUniform1f("u_Time", Timer::GetTime());
		shader->SetUniform1i("u_Texture", slot);

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, fb->GetColorAttachment());
		glActiveTexture(GL_TEXTURE0);

		glDrawElements(GL_TRIANGLES, ib->GetCount(), ib->GetType(), nullptr);
	}

	void Renderer::Submit(const Quad* quad, Shader* shader, const Framebuffer* fb, uint8_t slot)
	{
		quad->GetVertexBuffer()->Bind();
		const IndexBuffer* ib = Quad::GetIndexBuffer(); ib->Bind();
		shader->Bind(); fb->SetSlot(slot);

		shader->SetUniformMat4fv("u_ProjMat", camera->GetProjMat());
		shader->SetUniformMat4fv("u_ViewMat", camera->GetViewMat());
		shader->SetUniform1f("u_Time", Timer::GetTime());
		shader->SetUniform1i("u_Texture", slot);

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, fb->GetColorAttachment());
		glActiveTexture(GL_TEXTURE0);

		glDrawElements(GL_TRIANGLES, ib->GetCount(), ib->GetType(), nullptr);
	}

	void Renderer::Submit(const Quad* quad, Shader* shader)
	{
		quad->GetVertexBuffer()->Bind();
		const IndexBuffer* ib = Quad::GetIndexBuffer(); ib->Bind();
		shader->Bind();
		shader->SetUniformMat4fv("u_ProjMat", camera->GetProjMat());
		shader->SetUniformMat4fv("u_ViewMat", camera->GetViewMat());
		shader->SetUniform1f("u_Time", Timer::GetTime());

		glDrawElements(GL_TRIANGLES, ib->GetCount(), ib->GetType(), nullptr);
	}

	void Renderer::Submit(const Quad* quad, Shader* shader, const Texture* texture)
	{
		quad->GetVertexBuffer()->Bind();
		const IndexBuffer* ib = Quad::GetIndexBuffer(); ib->Bind();

		// Bind texture
		uint8_t slot = GetTextureSlot();
		texture->Bind(slot);

		// Bind shader
		shader->Bind();
		shader->SetUniform1i("u_Texture", slot);
		shader->SetUniformMat4fv("u_ProjMat", camera->GetProjMat());
		shader->SetUniformMat4fv("u_ViewMat", camera->GetViewMat());
		shader->SetUniform1f("u_Time", Timer::GetTime());

		glDrawElements(GL_TRIANGLES, ib->GetCount(), ib->GetType(), nullptr);

		// Unbind texture
		Texture::Unbind();
		FreeTextureSlot(slot);
	}

	void Renderer::Submit(const Quad* quad, Shader* shader, const Texture* texture, uint8_t slot)
	{
		quad->GetVertexBuffer()->Bind();
		const IndexBuffer* ib = Quad::GetIndexBuffer(); ib->Bind();

		// Bind texture
		texture->Bind(slot);

		// Bind shader
		shader->Bind();
		shader->SetUniform1i("u_Texture", slot);
		shader->SetUniformMat4fv("u_ProjMat", camera->GetProjMat());
		shader->SetUniformMat4fv("u_ViewMat", camera->GetViewMat());
		shader->SetUniform1f("u_Time", Timer::GetTime());

		glDrawElements(GL_TRIANGLES, ib->GetCount(), ib->GetType(), nullptr);
	}

	void Renderer::Submit(const Text* text)
	{
		uint8_t slot = Renderer::GetTextureSlot(); // Get slot we're drawing texture to

		// Bind shader and set texture slot
		text->shader->Bind();
		text->shader->SetUniform1i("u_Texture", slot);
		text->shader->SetUniformMat4fv("u_ProjMat", camera->GetProjMat());
		text->shader->SetUniform1f("u_Time", Timer::GetTime());

		// Bind texture to that slot
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindVertexArray(text->font->vertex_array_id);

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
			glBindTexture(GL_TEXTURE_2D, ch.texture_id);
			// Update content of vertex buffer memory
			glBindBuffer(GL_ARRAY_BUFFER, text->font->vertex_buffer_id);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			// Unbind buffer
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			rendering_pos.x += (ch.advance >> 6) * text->scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}
		// Unbind vertex array and texture
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Make texture slot available again
		Renderer::FreeTextureSlot(slot);
	}

	void Renderer::Submit(Button* btn)
	{
		// Get texture currently being used (if there is a texture)
		const Texture* current_texture = btn->CurrentTexture().get();
		if (current_texture != nullptr) {
			// Render with texture
			Renderer::Submit(&btn->quad, btn->CurrentShader().get(), current_texture);
		}
		else {
			// Render without texture
			Renderer::Submit(&btn->quad, btn->CurrentShader().get());
		}

		// Render text of button
		Renderer::Submit(btn->text);
	}

	void Renderer::Submit(Animation* animation)
	{
		Renderer::Submit(animation->quad.get(), animation->shader.get(), animation->GetAtlas().get());
	}

	void Renderer::Submit(TextInput* text_input)
	{
		if (text_input->bg_texture != nullptr) {
			Renderer::Submit(&text_input->quad, text_input->bg_shader.get(), text_input->bg_texture.get());
		}
		else {
			Renderer::Submit(&text_input->quad, text_input->bg_shader.get());
		}

		Renderer::Submit(text_input->GetText().get());

		// Display typing bar
		if (text_input->GetIsTyping()) {
			Renderer::Submit(text_input->GetTypingBar().get());
		}
	}

	void Renderer::Submit(Slider* slider)
	{
		slider->bar_shader->Bind();
		slider->bar_shader->SetUniform1f("u_Value", slider->GetValue());

		if (slider->bar_texture != nullptr) {
			Renderer::Submit(slider->m_BarQuad.get(), slider->bar_shader.get(), slider->bar_texture.get());
		}
		else {
			Renderer::Submit(slider->m_BarQuad.get(), slider->bar_shader.get());
		}

		if (slider->slider_texture != nullptr) {
			Renderer::Submit(slider->m_SliderQuad.get(), slider->slider_shader.get(), slider->slider_texture.get());
		}
		else {
			Renderer::Submit(slider->m_SliderQuad.get(), slider->slider_shader.get());
		}
	}

	void Renderer::Submit(Sprite* sprite)
	{
		Renderer::Submit(sprite->quad.get(), sprite->shader.get(), sprite->texture.get());
	}
}