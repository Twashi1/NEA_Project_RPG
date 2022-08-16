#pragma once

#include "Core.h"
#include "Texture.h"
#include "Quad.h"

namespace Vivium {
	// Allows easy access to texture coordinates of a specific texture, or textures in an atlas
	// Top left corner is (0,0), increasing as you go down and to the right
	class VIVIUM_API TextureAtlas {
	private:
		Ref(Texture) m_Atlas;
		Vector2<int> m_SpriteSize;
		Vector2<int> m_AtlasDimRelative;

		void m_Construct();

	public:
		std::vector<float> GetCoords(int index) const;
		std::vector<float> GetCoords(const Vector2<int>& index) const;
		std::vector<float> GetCoords(const Vector2<int>& top_left, const Vector2<int>& bottom_right) const;
		std::array<float, 8> GetCoordsArray(int index) const;
		std::array<float, 8> GetCoordsArray(const Vector2<int>& index) const;
		std::array<float, 8> GetCoordsArray(const Vector2<int>& top_left, const Vector2<int>& bottom_right) const;

		const Ref(Texture) GetAtlas() const;
		Vector2<int> GetSpriteSize() const;

		void Set(Quad* quad, int index) const;
		void Set(Quad* quad, const Vector2<int>& index) const;
		void Set(Quad* quad, const Vector2<int>& top_left, const Vector2<int>& bottom_right) const;

		TextureAtlas(Ref(Texture) atlas, const Vector2<int>& sprite_size);
		TextureAtlas(const Texture& atlas, const Vector2<int>& sprite_size);
		TextureAtlas(const std::string& atlas_path, const Vector2<int>& sprite_size);
	};
}
