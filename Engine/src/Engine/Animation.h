#pragma once

#include "Texture.h"
#include "Quad.h"
#include "Vector2.h"
#include "AnimationManager.h"

class ENGINE_API Animation {
	// Holds values for how much time should be spent on each frame before displaying the next
	std::vector<float> m_timings;

	int m_keyframes; // Amount of sprites there are in the animation
	Vector2<int> m_size; // Size of one sprite

	Vector2<int> m_atlas_dim_relative; // Dimensions of atlas in terms of sprites
	std::shared_ptr<Texture> m_atlas; // The texture atlas we got the sprites from

	// Parses some text to extract timtings and amount of keyframes
	void m_ParseData(const std::string& data_filename);
	// Get index in texture atlas of the current frame
	Vector2<int> m_GetIndex();

	float m_time = 0.0;
	float m_frame_time = 0.0; // Tracks time spent displaying the current frame
	int m_current_frame = 0; // Tracks the frame of the animation we're currently displaying

public:
	static std::string FILE_EXTENSION;

	std::shared_ptr<Quad> quad;
	std::shared_ptr<Shader> shader;

	const std::shared_ptr<Texture> GetAtlas() const;

	// NOTE: data_filename refers to the filename of the .animation data file for the texture atlas
	Animation(const std::shared_ptr<Quad>& quad, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& atlas, const Vector2<int>& size, const std::string& data_filename);
	Animation(const std::shared_ptr<Quad>& quad, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& atlas, const Vector2<int>& size, const std::vector<float>& animation_data);
	~Animation();

	// Updates the animation, takes current time
	void Update(float current_time);
};