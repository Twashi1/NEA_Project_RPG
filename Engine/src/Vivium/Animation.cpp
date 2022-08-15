#include "Animation.h"
namespace Vivium {
	std::string Animation::FILE_EXTENSION = "";

	Animation::Data::Data(const std::vector<float>& frame_timings)
	{
		// Iterate timing data passed
		for (float frame_time : frame_timings) {
			// If its a valid time value
			if (frame_time > 0.0) {
				this->frame_timings.push_back(frame_time);
			}
			// Otherwise increment starting index (since values of 0 indicate a sprite is not included in the animation)
			else {
				start_index++;
			}
		}
	}

	Animation::Data::Data(const std::string& animation_data_file)
	{
		// Load text
		std::string* filetext = Utils::ReadFile(Texture::PATH + animation_data_file + Animation::FILE_EXTENSION);

		// Split by c
		std::vector<std::string> parts = Utils::SplitString(*filetext, ",");

		int loop_start = 0;

		// Check if the first part of the data is a #n, indicating to skip n sprites forward
		if (parts[0][0] == '#') {
			// Erase first character (which is #)
			parts[0].erase(parts[0].begin());
			// Set our starting index to that value
			start_index = std::stoi(parts[0]);
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
				LogWarn("Invalid value in {}.animation: {}", animation_data_file, parts[i]);
			}

			// NOTE: Sometimes animations are included in larger texture atlases/multiple animations are in the same texture atlas,
			//	so we can enter lots of 0s at the start of our animation.data file so we start at the first frame of our animation

			// If value is more than 0, add the timing to our timings
			if (value > 0.0) {
				frame_timings.push_back(value);
			}
			// Otherwise increase the starting index
			else {
				start_index++;
			}
		}

		frame_count = frame_timings.size();

		delete filetext;
	}

	Vector2<int> Animation::m_GetIndex()
	{
		// Calculate y index
		int y = (m_FrameIndex + m_AnimationData.start_index) / m_AtlasDimRelative.x;
		// Calculate x index
		int x = (m_FrameIndex + m_AnimationData.start_index) - (y * m_AtlasDimRelative.x);
		// Invert y since we want to read top to bottom
		y = (m_AtlasDimRelative.y - 1) - y;

		// Return index
		return Vector2<int>(x, y);
	}

	void Animation::m_Construct()
	{
		// Calculate dimensions of atlas (in terms of sprites)
		m_AtlasDimRelative = Vector2<int>(m_Atlas->GetWidth() / m_SpriteSize.x, m_Atlas->GetHeight() / m_SpriteSize.y);
	}

	const std::shared_ptr<Texture> Animation::GetAtlas() const
	{
		return m_Atlas;
	}

	Animation::Animation(Ref(Quad) quad, Ref(Shader) shader, Ref(Texture) atlas, const Vector2<int>& sprite_size, const Animation::Data& animation_data)
		: quad(quad), shader(shader), m_Atlas(atlas), m_SpriteSize(sprite_size), m_AnimationData(animation_data)
	{
		m_Construct();
	}

	Animation::~Animation()
	{
	}

	void Animation::Update()
	{
		// Get elapsed time
		float elapsed = m_Timer.GetElapsed();

		// Add delta time to time spent on current frame
		m_FrameTime += elapsed;

		// Get time that is meant to be spent on this frame
		float this_frame_timing = m_AnimationData.frame_timings[m_FrameIndex];

		// If we have spent enough time on this frame
		if (m_FrameTime > this_frame_timing) {
			// Increment frame counter
			m_FrameIndex++;
			// If frame counter is more than amount of key frames, roll over
			if (m_FrameIndex >= m_AnimationData.frame_count) {
				m_FrameIndex = m_AnimationData.frame_count % m_FrameIndex;
			}
			// Reset current frame time
			m_FrameTime -= this_frame_timing;
			// Change texture coordinates
			quad->SetTextureCoords(*m_Atlas, m_GetIndex(), m_SpriteSize);
		}
	}
}