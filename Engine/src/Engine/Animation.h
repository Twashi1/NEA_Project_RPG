#pragma once

#include "Texture.h"
#include "Quad.h"
#include "Vector2.h"
#include "AnimationManager.h"

// TODO: python scripting for animations? or python script to generate timings for animations?
// TODO: better names

class ENGINE_API Animation {
private:
	// Holds values for minimum time to be spent on each frame before displaying the next
	std::vector<float> m_FrameTimes;

	int m_FrameCount; // Amount of sprites there are in the animation
	Vector2<int> m_SpriteSize; // Size of one sprite

	Vector2<int> m_AtlasDimRelative; // Dimensions of atlas in terms of sprites
	ENG_Ptr(Texture) m_Atlas; // The texture atlas we got the sprites from

	// Parses some text to extract timtings and amount of keyframes
	void m_ParseData(const std::string& data_filename);
	// Get index in texture atlas of the current frame
	Vector2<int> m_GetIndex();

	void m_Construct();

	float m_Time = 0.0;
	float m_FrameTime = 0.0; // Tracks time spent displaying the current frame
	int m_FrameIndex = 0; // Tracks the frame of the animation we're currently displaying
	int m_StartIndex = 0; // Tracks the index in the texture atlas where we're mean to start the animation

public:
	static std::string FILE_EXTENSION;

	ENG_Ptr(Quad) quad;
	ENG_Ptr(Shader) shader;

	const ENG_Ptr(Texture) GetAtlas() const;

	// NOTE: data_filename refers to the filename of the .animation data file for the texture atlas
	Animation(const std::shared_ptr<Quad>& quad, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& atlas, const Vector2<int>& sprite_size, const std::string& data_filename);
	Animation(const std::shared_ptr<Quad>& quad, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& atlas, const Vector2<int>& sprite_size, const std::vector<float>& animation_data);
	~Animation();

	// Updates the animation, takes current time
	void Update(float current_time);
};