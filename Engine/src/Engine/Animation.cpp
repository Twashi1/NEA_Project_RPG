#include "Animation.h"

std::string Animation::FILE_EXTENSION = "";

void Animation::m_ParseData(const std::string& data_filename)
{
	std::string* filetext = Utils::ReadFile(Texture::PATH + data_filename + Animation::FILE_EXTENSION);

	std::vector<std::string> parts = Utils::SplitString(*filetext, ",");
	
	m_keyframes = parts.size();

	// Iterate over each string
	for (int i = 0; i < parts.size(); i++) {
		float value = 0.0;

		try {
			// Convert value to float
			value = std::stof(parts[i]);
		}
		// If animation file contains value that cannot be converted to float
		catch (std::invalid_argument) {
			std::string text = std::format("Invalid value in {}.animation: {}", data_filename, parts[i]);
			Log(text, Utils::ERROR::WARNING);
		}

		m_timings.push_back(value);
	}

	delete filetext;
}

Vector2<int> Animation::m_GetIndex()
{
	// Calculate y index
	int y = m_current_frame / m_atlas_dim_relative.x;
	// Calculate x index
	int x = m_current_frame - (y * m_atlas_dim_relative.x);
	// Invert y since we want to read top to bottom
	y = (m_atlas_dim_relative.y - 1) - y;

	// Return index
	return Vector2<int>(x, y);
}

Animation::Animation(std::shared_ptr<Renderable> renderable, std::shared_ptr<Texture> atlas, const Vector2<int>& size, std::string data_filename)
	: renderable(renderable), m_atlas(atlas), m_size(size)
{
	// Parse animation file
	m_ParseData(data_filename);

	// Calculate dimensions of atlas (in terms of sprites)
	m_atlas_dim_relative = Vector2<int>(atlas->width / m_size.x, atlas->height / m_size.y);

	// Add ourselves to animation manager
	AnimationManager::animations.Push(this);
}

Animation::~Animation()
{
	// Remove ourselves from animation manager
	AnimationManager::animations.Remove(this);
}

void Animation::Update(float current_time)
{
	float dt = current_time - m_time; // Calculate deltatime

	// Add delta time to time spent on current frame
	m_frame_time += dt;

	// Get time that is meant to be spent on this frame
	float this_frame_timing = m_timings[m_current_frame];
	
	// If we have spent enough time on this frame
	if (m_frame_time > this_frame_timing) {
		// Increment frame counter
		m_current_frame++;
		// If frame counter is more than amount of key frames, roll over
		if (m_current_frame >= m_keyframes) {
			m_current_frame = m_keyframes % m_current_frame;
		}
		// Reset current frame time
		m_frame_time -= this_frame_timing;
		// Change texture coordinates
		renderable->quad->SetTextureCoords(*m_atlas, m_GetIndex(), m_size);
	}

	// Update my time
	m_time = current_time;
}
