#pragma once

#include "Utils.h"
#include "Texture.h"
#include "Sprite.h"

/*


*/

namespace Vivium {
	// Allows easy access to texture coordinates of a specific texture, or textures in an atlas
	// Top left corner is (0,0), increasing as you go down and to the right
	class ENGINE_API TextureAtlas {
	private:
		ENG_Ptr(Texture) m_Atlas;
		Vector2<int> m_SpriteSize;
		Vector2<int> m_AtlasDimRelative;

		typedef std::vector<float> TexCoords_t;

		void m_Set(Quad& quad, const Vector2<int>& index) const;
		void m_Construct();

	public:
		//const TexCoords_t& GetCoords(const Vector2<int>& index) const;
		//const TexCoords_t& GetCoords(const Vector2<int>& top_left, const Vector2<int>& bottom_right) const;

		const ENG_Ptr(Texture) GetAtlas() const;

		//void Set(Sprite& sprite, const Vector2<int>& index) const;
		//void Set(Sprite& sprite, const Vector2<int>& top_left, const Vector2<int>& bottom_right) const;
		//void Set(Quad& quad, const Vector2<int>& index) const;
		//void Set(Quad& quad, const Vector2<int>& index) const;

		TextureAtlas(ENG_Ptr(Texture) atlas, const Vector2<int>& sprite_size);
		TextureAtlas(const Texture& atlas, const Vector2<int>& sprite_size);
		TextureAtlas(const std::string& atlas_path, const Vector2<int>& sprite_size);
	};
}
