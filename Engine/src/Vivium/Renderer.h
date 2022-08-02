#pragma once

#include "Core.h"
#include "Logger.h"

#include "Framebuffer.h"

namespace Vivium {
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	struct Camera;
	class Text;
	class Button;
	class TextInput;
	class Animation;
	struct Quad;
	class Texture;
	class Slider;
	class ToggleSwitch;

	class VIVIUM_API Renderer {
	private:
		static std::vector<uint8_t> m_AvailableSlots; // Available slots for texture

	public:
		static Ref(Camera) camera; // Camera currently being used on all Renderable objects

		static constexpr uint8_t INVALID_TEXTURE_SLOT = 0xff; // Constant to represent an invalid texture slot
		static constexpr uint8_t MAX_TEXTURE_SLOT = 0xf;

		static void Init();

		// Gives a free texture slot
		static uint8_t GetTextureSlot();
		// Makes a texture slot available again
		static void FreeTextureSlot(uint8_t slot);

		static void SetTarget(const Framebuffer* fb);
		static void ResetTarget();

		static void Submit(const VertexBuffer* vb, const IndexBuffer* ib, Shader* shader);
		static void Submit(const VertexBuffer* vb, const IndexBuffer* ib, Shader* shader, Texture* texture, uint8_t slot = 0);
		static void Submit(const VertexBuffer* vb, const IndexBuffer* ib, Shader* shader, const Framebuffer* fb, uint8_t slot = 0);
		static void Submit(const Quad* quad, Shader* shader);
		static void Submit(const Quad* quad, Shader* shader, const Texture* texture);
		static void Submit(const Quad* quad, Shader* shader, const Texture* texture, uint8_t slot);
		static void Submit(const Text* text);
		static void Submit(Button* btn);
		static void Submit(Animation* animation);
		static void Submit(TextInput* text_input);
		static void Submit(Slider* slider);
		static void Submit(ToggleSwitch* toggle_switch);
	};
}
