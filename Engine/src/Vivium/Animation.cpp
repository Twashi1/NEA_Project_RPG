#include "Animation.h"
namespace Vivium {
	std::string Animation::FILE_EXTENSION = "";

	Animation::Data::Data(const std::vector<float>& frame_timings)
	{
		VIVIUM_SCOPE;

		int sprite_index = 0;

		// Iterate timing data passed
		for (float frame_time : frame_timings) {
			// If its a valid time value
			if (frame_time > 0.0) {
				this->frame_timings.push_back(frame_time);
				++frame_count;
				frame_indices.push_back(sprite_index);
			}

			++sprite_index;
		}
	}

	Animation::Data::Data(const std::vector<float>& frame_timings, const std::vector<int>& frame_indices)
		: frame_timings(frame_timings), frame_indices(frame_indices), frame_count(frame_indices.size())
	{}

	Animation::Data::Data(const std::string& animation_data_file, const Vector2<int>& atlas_dim)
	{
		VIVIUM_SCOPE;

		// Load text
		std::string* filetext = Utils::ReadFile(Texture::PATH + animation_data_file + Animation::FILE_EXTENSION);

		// Split by comma
		std::vector<std::string> parts = Utils::SplitString(*filetext, ",");

		int loop_start = 0;
		int sprite_index = 0;
		bool indices_list = false;

		// Check if the first part of the data is a #n, indicating to skip n sprites forward
		if (parts[0][0] == '#') {
			// Erase first character (which is #)
			parts[0].erase(parts[0].begin());
			// Set our starting index to that value
			sprite_index = std::stoi(parts[0]);
			// Start at next index since the first isn't a float value
			loop_start = 1;
		}

		// Indicating we are entering a list of indices
		if (parts[0][0] == '{') {
			indices_list = true;
			
			LogTrace("Detected indices list");
			
			// TODO: So unreadable
			for (int i = 0; i < parts.size(); i += 2) {
				std::string index_str = parts[i] + "," + parts[i + 1];
				// TODO: use regex next time
				Utils::EraseSubstring(index_str, "{");
				Utils::EraseSubstring(index_str, "}\n");

				if (index_str[0] != '(') break;

				Utils::EraseSubstring(index_str, "(");
				Utils::EraseSubstring(index_str, ")");
				
				std::vector<std::string> index_split = Utils::SplitString(index_str, ",");
				int x_index = std::stoi(index_split[0]);
				int y_index = std::stoi(index_split[1]);
				int index_1d = x_index + y_index * atlas_dim.x;

				frame_indices.push_back(index_1d);
			}

			frame_count = frame_indices.size();
			loop_start = frame_indices.size() * 2;
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
				++frame_count;
				if (!indices_list) { frame_indices.push_back(sprite_index); }
			}
			
			++sprite_index;
		}

		delete filetext;
	}

	void Animation::Data::Write(Vivium::Serialiser& s) const
	{
		s.Write(frame_timings);
		s.Write(frame_indices);
		s.Write(frame_count);
	}

	void Animation::Data::Read(Vivium::Serialiser& s)
	{
		s.Read(&frame_timings);
		s.Read(&frame_indices);
		s.Read(&frame_count);
	}

	const std::shared_ptr<Texture> Animation::GetAtlas() const
	{
		return m_Atlas->GetAtlas();
	}

	Animation::Animation(std::shared_ptr<Quad> quad, std::shared_ptr<Shader> shader, std::shared_ptr<TextureAtlas> atlas, const Animation::Data& animation_data)
		: quad(quad), shader(shader), m_Atlas(atlas), m_AnimationData(animation_data)
	{}

	Animation::~Animation() {}

	void Animation::Update()
	{
		VIVIUM_SCOPE;

		// Get elapsed time
		float elapsed = m_Timer.GetElapsed();

		// Add delta time to time spent on current frame
		m_FrameTime += elapsed;

		// Get time that is meant to be spent on this frame
		float this_frame_timing = m_AnimationData.frame_timings[m_FrameIndex];

		// If we have spent enough time on this frame
		if (m_FrameTime > this_frame_timing) {
			// Increment frame counter
			++m_FrameIndex;
			m_FrameIndex %= m_AnimationData.frame_count;
			// Reset current frame time
			m_FrameTime -= this_frame_timing;
			// Change texture coordinates
			m_Atlas->Set(quad.get(), m_AnimationData.frame_indices[m_FrameIndex]);
		}
	}
}