#include "TextureAtlas.h"

namespace Vivium {
	void TextureAtlas::m_Set(Quad& quad, const Vector2<int>& index) const
	{

	}

	void TextureAtlas::m_Construct()
	{
		m_AtlasDimRelative = Vector2<int>(m_Atlas->width / m_SpriteSize.x, m_Atlas->height / m_SpriteSize.y);
	}

	const Ref(Texture) TextureAtlas::GetAtlas() const
	{
		return m_Atlas;
	}

	TextureAtlas::TextureAtlas(Ref(Texture) atlas, const Vector2<int>& sprite_size)
		: m_Atlas(atlas), m_SpriteSize(sprite_size)
	{
		m_Construct();
	}

	TextureAtlas::TextureAtlas(const Texture& atlas, const Vector2<int>& sprite_size)
		: m_Atlas(MakeRef(Texture, atlas)), m_SpriteSize(sprite_size)
	{
		m_Construct();
	}
	TextureAtlas::TextureAtlas(const std::string& atlas_path, const Vector2<int>& sprite_size)
		: m_Atlas(MakeRef(Texture, atlas_path)), m_SpriteSize(sprite_size)
	{
		m_Construct();
	}
}
