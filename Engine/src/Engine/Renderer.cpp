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
#include "SceneObject.h"
#include "Slider.h"
#include "ToggleSwitch.h"

namespace CoolEngineName {
	void GlClearError() {
		// While we have an error stay in function
		int max = 0;
		while (glGetError() != GL_NO_ERROR && max < 100) { max++; }
	}

	bool GlLogCall(const char* function, const char* file, int line) {
		while (GLenum error = glGetError()) {
			std::cout << "[OpenGL Error @ " << function << "]: {" << error << ": " << glewGetErrorString(error) << "} " << file << ":" << line << std::endl;
			return false;
		}
		return true;
	}

	std::vector<uint8_t> Renderer::m_AvailableSlots = std::vector<uint8_t>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
	ENG_Ptr(Camera) Renderer::camera = nullptr;

	void Renderer::Init()
	{
		camera = ENG_MakePtr(Camera, 0.0f, (float)Application::width, 0.0f, (float)Application::height);
	}

	ENGINE_API uint8_t Renderer::GetTextureSlot()
	{
		if (m_AvailableSlots.size() > 1) {
			// Get front of queue and store it
			uint8_t id = m_AvailableSlots.front();
			// Remove that id from list
			m_AvailableSlots.pop_back();
			// Return that id
			return id;
		}

		ENG_LogInfo("Ran out of texture slots (MAX 16 concurrent)");
		return Renderer::m_INVALID_TEXTURE_SLOT;
	}

	ENGINE_API void Renderer::FreeTextureSlot(uint8_t slot)
	{
		if (slot != Renderer::m_INVALID_TEXTURE_SLOT) {
			m_AvailableSlots.emplace_back(slot);
		}
	}

	ENGINE_API void Renderer::Schedule(const Quad* quad, Shader* shader)
	{
		quad->GetVertexBuffer().Bind();
		const IndexBuffer& ib = Quad::GetIndexBuffer(); ib.Bind();
		shader->Bind();
		shader->SetUniformMat4fv("u_ProjMat", camera->GetProjMat());
		shader->SetUniformMat4fv("u_ViewMat", camera->GetViewMat());
		shader->SetUniform1f("u_Time", Utils::Timer::GetTime());

		GlCall(glDrawElements(GL_TRIANGLES, ib.count, ib.type, nullptr));
	}

	ENGINE_API void Renderer::Schedule(const Quad* quad, Shader* shader, const Texture* texture)
	{
		quad->GetVertexBuffer().Bind();
		const IndexBuffer& ib = Quad::GetIndexBuffer(); ib.Bind();

		// Bind texture
		uint8_t slot = GetTextureSlot();
		texture->Bind(slot);

		// Bind shader
		shader->Bind();
		shader->SetUniform1i("u_Texture", slot);
		shader->SetUniformMat4fv("u_ProjMat", camera->GetProjMat());
		shader->SetUniformMat4fv("u_ViewMat", camera->GetViewMat());
		shader->SetUniform1f("u_Time", Utils::Timer::GetTime());

		GlCall(glDrawElements(GL_TRIANGLES, ib.count, ib.type, nullptr));

		// Unbind texture
		Texture::Unbind();
		FreeTextureSlot(slot);
	}

	ENGINE_API void Renderer::Schedule(const Quad* quad, Shader* shader, const Texture* texture, uint8_t slot)
	{
		quad->GetVertexBuffer().Bind();
		const IndexBuffer& ib = Quad::GetIndexBuffer(); ib.Bind();

		// Bind texture
		texture->Bind(slot);

		// Bind shader
		shader->Bind();
		shader->SetUniform1i("u_Texture", slot);
		shader->SetUniformMat4fv("u_ProjMat", camera->GetProjMat());
		shader->SetUniformMat4fv("u_ViewMat", camera->GetViewMat());
		shader->SetUniform1f("u_Time", Utils::Timer::GetTime());

		GlCall(glDrawElements(GL_TRIANGLES, ib.count, ib.type, nullptr));
	}

	ENGINE_API void Renderer::Schedule(const Text* text)
	{
		uint8_t slot = Renderer::GetTextureSlot(); // Get slot we're drawing texture to

		// Bind shader and set texture slot
		text->shader->Bind();
		text->shader->SetUniform1i("u_Texture", slot);
		text->shader->SetUniformMat4fv("u_ProjMat", camera->GetProjMat());
		text->shader->SetUniform1f("u_Time", Utils::Timer::GetTime());

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

	ENGINE_API void Renderer::Schedule(Button* btn)
	{
		// Get texture currently being used (if there is a texture)
		const Texture* current_texture = btn->CurrentTexture().get();
		if (current_texture != nullptr) {
			// Render with texture
			Renderer::Schedule(&btn->quad, btn->CurrentShader().get(), current_texture);
		}
		else {
			// Render without texture
			Renderer::Schedule(&btn->quad, btn->CurrentShader().get());
		}

		// Render text of button
		Renderer::Schedule(btn->text);
	}

	ENGINE_API void Renderer::Schedule(Animation* animation)
	{
		Renderer::Schedule(animation->quad.get(), animation->shader.get(), animation->GetAtlas().get());
	}

	ENGINE_API void Renderer::Schedule(TextInput* text_input)
	{
		if (text_input->bg_texture != nullptr) {
			Renderer::Schedule(&text_input->quad, text_input->bg_shader.get(), text_input->bg_texture.get());
		}
		else {
			Renderer::Schedule(&text_input->quad, text_input->bg_shader.get());
		}

		Renderer::Schedule(text_input->GetText().get());

		// Display typing bar
		if (text_input->GetIsTyping()) {
			Renderer::Schedule(text_input->GetTypingBar().get());
		}
	}

	ENGINE_API void Renderer::Schedule(SceneObject* scene_object)
	{
		switch (scene_object->m_Type) {
		case SceneObject::Type::SHADED_QUAD:
			Renderer::Schedule(scene_object->shaded_quad.quad.get(), scene_object->shaded_quad.shader.get()); break;
		case SceneObject::Type::TEXTURED_QUAD:
			Renderer::Schedule(scene_object->textured_quad.quad.get(), scene_object->textured_quad.shader.get(), scene_object->textured_quad.texture.get()); break;
		case SceneObject::Type::ANIMATION:
			Renderer::Schedule(scene_object->animation.get());  break;
		case SceneObject::Type::BUTTON:
			Renderer::Schedule(scene_object->button.get()); break;
		case SceneObject::Type::TEXT_INPUT:
			Renderer::Schedule(scene_object->text_input.get()); break;
		case SceneObject::Type::TEXT:
			Renderer::Schedule(scene_object->text.get()); break;
		default:
			ENG_LogWarn("SceneObject has invalid type {}", to_string(scene_object->m_Type));
		}
	}

	ENGINE_API void Renderer::Schedule(Slider* slider)
	{
		slider->bar_shader->Bind();
		slider->bar_shader->SetUniform1f("u_Value", slider->GetValue());

		if (slider->bar_texture != nullptr) {
			Renderer::Schedule(slider->m_BarQuad.get(), slider->bar_shader.get(), slider->bar_texture.get());
		}
		else {
			Renderer::Schedule(slider->m_BarQuad.get(), slider->bar_shader.get());
		}

		if (slider->slider_texture != nullptr) {
			Renderer::Schedule(slider->m_SliderQuad.get(), slider->slider_shader.get(), slider->slider_texture.get());
		}
		else {
			Renderer::Schedule(slider->m_SliderQuad.get(), slider->slider_shader.get());
		}
	}

	ENGINE_API void Renderer::Schedule(ToggleSwitch* toggle_switch)
	{
		Shader* shader = toggle_switch->GetShader();
		Texture* texture = toggle_switch->GetTexture();

		if (texture != nullptr) {
			Renderer::Schedule(toggle_switch->quad.get(), shader, texture);
		}
		else {
			Renderer::Schedule(toggle_switch->quad.get(), shader);
		}
	}
}