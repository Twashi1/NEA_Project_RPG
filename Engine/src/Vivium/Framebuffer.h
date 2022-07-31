#pragma once

#include "Core.h"
#include "Logger.h"
#include "Math.h"

namespace Vivium {
	class VIVIUM_API Framebuffer {


		// TODO private
	private:
		int width, height;
		GLuint id;
		GLuint color_attachment;

	public:
		Framebuffer(int width, int height);
		~Framebuffer();

		void SetSlot(unsigned int slot);

		int GetWidth() const;
		int GetHeight() const;
		Vector2<int> GetDim() const;
		GLuint GetColorAttachment() const;

		void Bind() const;
		static void Unbind();

		void Resize(int width, int height);
	};
}
