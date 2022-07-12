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
		m_start_index = std::stoi(parts[0]);
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
			std::string text = std::format("Invalid value in {}.animation: {}", data_filename, parts[i]);
			Log(text, LOG::WARNING);
		}

		// NOTE: Sometimes animations are included in larger texture atlases/multiple animations are in the same texture atlas,
		//	so we can enter lots of 0s at the start of our animation.data file so we start at the first frame of our animation
		
		// If value is more than 0, add the timing to our timings
		if (value > 0.0) {
			m_timings.push_back(value);
		}
		// Otherwise increase the starting index
		else {
			m_start_index++;
		}
	}

	m_keyframes = m_timings.size();

	delete filetext;
}

Vector2<int> Animation::m_GetIndex()
{
	// Calculate y index
	int y = (m_current_frame + m_start_index) / m_atlas_dim_relative.x;
	// Calculate x index
	int x = (m_current_frame + m_start_index) - (y * m_atlas_dim_relative.x);
	// Invert y since we want to read top to bottom
	y = (m_atlas_dim_relative.y - 1) - y;

	// Return index
	return Vector2<int>(x, y);
}

const std::shared_ptr<Texture> Animation::GetAtlas() const
{
	return m_atlas;
}

Animation::Animation(const std::shared_ptr<Quad>& quad, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& atlas, const Vector2<int>& size, const std::string& data_filename)
	: quad(quad), shader(shader), m_atlas(atlas), m_size(size)
{
	// Parse animation file
	m_ParseData(data_filename);

	// Calculate dimensions of atlas (in terms of sprites)
	m_atlas_dim_relative = Vector2<int>(atlas->width / m_size.x, atlas->height / m_size.y);

	// Add ourselves to animation manager
	AnimationManager::animations.Push(this);
}

Animation::Animation(const std::shared_ptr<Quad>& quad, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& atlas, const Vector2<int>& size, const std::vector<float>& animation_data)
	: quad(quad), shader(shader), m_atlas(atlas), m_size(size)
{
	// Iterate timing data passed
	for (float timing : animation_data) {
		if (timing > 0.0) {
			m_timings.push_back(timing);
		}
		else {
			m_start_index++;
		}
	}

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
	float elapsed = current_time - m_time; // Calculate deltatime

	// Add delta time to time spent on current frame
	m_frame_time += elapsed;

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
		quad->SetTextureCoords(*m_atlas, m_GetIndex(), m_size);
	}

	// Update my time
	m_time = current_time;
}
