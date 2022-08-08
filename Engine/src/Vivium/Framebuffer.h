#pragma once

#include "Core.h"
#include "Logger.h"
#include "Math.h"

namespace Vivium {
	class Renderer;

	class VIVIUM_API Framebuffer {
	private:
		int m_Width, m_Height;
		GLuint m_ID;
		GLuint m_ColorAttachment;

	public:
		Framebuffer(int width, int height);
		~Framebuffer();

		void SetSlot(unsigned int slot) const;

		int GetWidth() const;
		int GetHeight() const;
		Vector2<int> GetDim() const;
		GLuint GetColorAttachment() const;

		void Clear() const;
		void Bind() const;
		static void Unbind();

		void Resize(int width, int height);

		friend Renderer;
	};
}
