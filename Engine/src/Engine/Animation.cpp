#include "Animation.h"

std::string Animation::FILE_EXTENSION = "";

void Animation::m_ParseData(const std::string& data_filename)
{
	std::string* filetext = Utils::ReadFile(Texture::PATH + data_filename + Animation::FILE_EXTENSION);

	std::vector<std::string> parts = Utils::SplitString(*filetext, ",");

	int loop_start = 0;

	// Check if the first part of the data is a #n, indicating to skip n sprites forward
	if (parts[0][0] == '#') {
		// Erase first character (which is #)
		parts[0].erase(parts[0].begin());
		// Set our starting index to that value
		m_StartIndex = std::stoi(parts[0]);
		// Start at next index since the first isn't a float value
		loop_start = 1;
	}

	// Iterate over each string
	for (int i = loop_start; i < parts.size(); i++) {
		float value = 0.0;

		try {
			// Convert value to float
			value = std::stof(parts[i]);
		}
		// If animation file contains value that cannot be converted to float
		catch (std::invalid_argument) {
			ENG_LogWarn("Invalid value in {}.animation: {}", data_filename, parts[i]);
		}

		// NOTE: Sometimes animations are included in larger texture atlases/multiple animations are in the same texture atlas,
		//	so we can enter lots of 0s at the start of our animation.data file so we start at the first frame of our animation
		
		// If value is more than 0, add the timing to our timings
		if (value > 0.0) {
			m_FrameTimes.push_back(value);
		}
		// Otherwise increase the starting index
		else {
			m_StartIndex++;
		}
	}

	m_FrameCount = m_FrameTimes.size();

	delete filetext;
}

Vector2<int> Animation::m_GetIndex()
{
	// Calculate y index
	int y = (m_FrameIndex + m_StartIndex) / m_AtlasDimRelative.x;
	// Calculate x index
	int x = (m_FrameIndex + m_StartIndex) - (y * m_AtlasDimRelative.x);
	// Invert y since we want to read top to bottom
	y = (m_AtlasDimRelative.y - 1) - y;

	// Return index
	return Vector2<int>(x, y);
}

void Animation::m_Construct()
{
	// Calculate dimensions of atlas (in terms of sprites)
	m_AtlasDimRelative = Vector2<int>(m_Atlas->width / m_SpriteSize.x, m_Atlas->height / m_SpriteSize.y);

	// Add ourselves to animation manager
	AnimationManager::animations.push_back(this);
}

const std::shared_ptr<Texture> Animation::GetAtlas() const
{
	return m_Atlas;
}

Animation::Animation(const std::shared_ptr<Quad>& quad, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& atlas, const Vector2<int>& sprite_size, const std::string& data_filename)
	: quad(quad), shader(shader), m_Atlas(atlas), m_SpriteSize(sprite_size)
{
	// Parse animation file
	m_ParseData(data_filename);

	m_Construct();
}

Animation::Animation(const std::shared_ptr<Quad>& quad, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& atlas, const Vector2<int>& sprite_size, const std::vector<float>& animation_data)
	: quad(quad), shader(shader), m_Atlas(atlas), m_SpriteSize(sprite_size)
{
	// Iterate timing data passed
	for (float frame_time : animation_data) {
		// If its a valid time value
		if (frame_time > 0.0) {
			m_FrameTimes.push_back(frame_time);
		}
		// Otherwise increment starting index (since values of 0 indicate a sprite is not included in the animation)
		else {
			m_StartIndex++;
		}
	}

	m_Construct();
}

Animation::~Animation()
{
	// Remove ourselves from animation manager
	Utils::Remove(AnimationManager::animations, this);
}

void Animation::Update(float current_time)
{
	float elapsed = current_time - m_Time; // Calculate deltatime

	// Add delta time to time spent on current frame
	m_FrameTime += elapsed;

	// Get time that is meant to be spent on this frame
	float this_frame_timing = m_FrameTimes[m_FrameIndex];
	
	// If we have spent enough time on this frame
	if (m_FrameTime > this_frame_timing) {
		// Increment frame counter
		m_FrameIndex++;
		// If frame counter is more than amount of key frames, roll over
		if (m_FrameIndex >= m_FrameCount) {
			m_FrameIndex = m_FrameCount % m_FrameIndex;
		}
		// Reset current frame time
		m_FrameTime -= this_frame_timing;
		// Change texture coordinates
		quad->SetTextureCoords(*m_Atlas, m_GetIndex(), m_SpriteSize);
	}

	// Update my time
	m_Time = current_time;
}
