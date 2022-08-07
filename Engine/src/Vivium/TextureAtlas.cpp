#include "TextureAtlas.h"

namespace Vivium {
	void TextureAtlas::Set(Quad* quad, int index) const
	{
		// Inverse width and height of atlas
		float inv_width = 1.0f / m_Atlas->width;
		float inv_height = 1.0f / m_Atlas->height;

		int y_index = index / m_AtlasDimRelative.x;
		int x_index = index - y_index * m_AtlasDimRelative.x;
		y_index = m_AtlasDimRelative.y - 1 - y_index; // Flip vertically TODO: variable for this

		// Calculate faces
		float left = x_index * inv_width * m_SpriteSize.x;
		float right = (x_index + 1) * inv_width * m_SpriteSize.x;
		float bottom = y_index * inv_height * m_SpriteSize.y;
		float top = (y_index + 1) * inv_height * m_SpriteSize.y;

		// Create tex coords list
		float tex_coords[] = {
				left, bottom,
				right, bottom,
				right, top,
				left, top
		};

		// Update texture coords
		quad->vb->Set(1, tex_coords, sizeof(float) * 8);
	}

	void TextureAtlas::Set(Quad* quad, const Vector2<int>& index) const
	{
		// Inverse width and height of atlas
		float inv_width = 1.0f / m_Atlas->width;
		float inv_height = 1.0f / m_Atlas->height;

		// Calculate faces
		float left = index.x * inv_width * m_SpriteSize.x;
		float right = (index.x + 1) * inv_width * m_SpriteSize.x;
		float bottom = index.y * inv_height * m_SpriteSize.y;
		float top = (index.y + 1) * inv_height * m_SpriteSize.y;

		// Create tex coords list
		float tex_coords[] = {
				left, bottom,
				right, bottom,
				right, top,
				left, top
		};

		// Update texture coords
		quad->vb->Set(1, tex_coords, sizeof(float) * 8);
	}

	void TextureAtlas::Set(Quad* quad, const Vector2<int>& top_left, const Vector2<int>& bottom_right) const
	{
		// Inverse width and height of atlas
		float inv_width = 1.0f / m_Atlas->width;
		float inv_height = 1.0f / m_Atlas->height;

		// Calculate faces
		float left = top_left.x * inv_width * m_SpriteSize.x;
		float right = bottom_right.x * inv_width * m_SpriteSize.x;
		float bottom = bottom_right.y * inv_height * m_SpriteSize.y;
		float top = top_left.y * inv_height * m_SpriteSize.y;

		// Create tex coords list
		float tex_coords[] = {
				left, bottom,
				right, bottom,
				right, top,
				left, top
		};

		// Update texture coords
		quad->vb->Set(1, tex_coords, sizeof(float) * 8);
	}

	void TextureAtlas::m_Construct()
	{
		m_AtlasDimRelative = Vector2<int>(m_Atlas->width / m_SpriteSize.x, m_Atlas->height / m_SpriteSize.y);
	}

	std::vector<float> TextureAtlas::GetCoords(int index) const
	{
		// Inverse width and height of atlas
		float inv_width = 1.0f / m_Atlas->width;
		float inv_height = 1.0f / m_Atlas->height;

		int y_index = index / m_AtlasDimRelative.x;
		int x_index = index - y_index * m_AtlasDimRelative.x;
		y_index = m_AtlasDimRelative.y - 1 - y_index; // Flip vertically TODO: variable for this

		// Calculate faces
		float left = x_index * inv_width * m_SpriteSize.x;
		float right = (x_index + 1) * inv_width * m_SpriteSize.x;
		float bottom = y_index * inv_height * m_SpriteSize.y;
		float top = (y_index + 1) * inv_height * m_SpriteSize.y;

		// Create tex coords list
		std::vector<float> tex_coords = {
				left, bottom,
				right, bottom,
				right, top,
				left, top
		};

		// Update texture coords
		return tex_coords;
	}

	std::vector<float> TextureAtlas::GetCoords(const Vector2<int>& index) const
	{
		// Inverse width and height of atlas
		float inv_width = 1.0f / m_Atlas->width;
		float inv_height = 1.0f / m_Atlas->height;

		// Calculate faces
		float left = index.x * inv_width * m_SpriteSize.x;
		float right = (index.x + 1) * inv_width * m_SpriteSize.x;
		float bottom = index.y * inv_height * m_SpriteSize.y;
		float top = (index.y + 1) * inv_height * m_SpriteSize.y;

		// Create tex coords list
		std::vector<float> tex_coords = {
				left, bottom,
				right, bottom,
				right, top,
				left, top
		};

		// Update texture coords
		return tex_coords;
	}

	std::vector<float> TextureAtlas::GetCoords(const Vector2<int>& top_left, const Vector2<int>& bottom_right) const
	{
		// Inverse width and height of atlas
		float inv_width = 1.0f / m_Atlas->width;
		float inv_height = 1.0f / m_Atlas->height;

		// Calculate faces
		float left = top_left.x * inv_width * m_SpriteSize.x;
		float right = bottom_right.x * inv_width * m_SpriteSize.x;
		float bottom = bottom_right.y * inv_height * m_SpriteSize.y;
		float top = top_left.y * inv_height * m_SpriteSize.y;

		// Create tex coords list
		std::vector<float> tex_coords = {
				left, bottom,
				right, bottom,
				right, top,
				left, top
		};

		// Update texture coords
		return tex_coords;
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
