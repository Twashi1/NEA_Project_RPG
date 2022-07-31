#pragma once

#include "Core.h"
#include "Logger.h"

namespace Vivium {
	struct VertexBuffer;
	struct IndexBuffer;
	class Shader;
	struct Camera;
	class Text;
	class Button;
	class TextInput;
	class Animation;
	struct Quad;
	class Texture;
	struct SceneObject;
	class Slider;
	class ToggleSwitch;

	class VIVIUM_API Renderer {
	private:
		static constexpr uint8_t m_INVALID_TEXTURE_SLOT = 255; // Constant to represent an invalid texture slot
		static std::vector<uint8_t> m_AvailableSlots; // Available slots on GPU for texture

	public:
		static Ref(Camera) camera; // Camera currently being used on all Renderable objects

		static void Init();

		// Gives a free texture slot
		static uint8_t GetTextureSlot();
		// Makes a texture slot available again
		static void FreeTextureSlot(uint8_t slot);

		static void Schedule(const Quad* quad, Shader* shader);
		static void Schedule(const Quad* quad, Shader* shader, const Texture* texture);
		static void Schedule(const Quad* quad, Shader* shader, const Texture* texture, uint8_t slot);
		static void Schedule(const Text* text);
		static void Schedule(Button* btn);
		static void Schedule(Animation* animation);
		static void Schedule(TextInput* text_input);
		static void Schedule(SceneObject* scene_object);
		static void Schedule(Slider* slider);
		static void Schedule(ToggleSwitch* toggle_switch);
	};
}
