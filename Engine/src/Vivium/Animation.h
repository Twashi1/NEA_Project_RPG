#pragma once

#include "Utils.h"
#include "Logger.h"
#include "Vector2.h"
#include "Quad.h"
#include "Texture.h"
#include "TextureAtlas.h"

// TODO: python scripting for animations? or python script to generate timings for animations?
// TODO: better names

namespace Vivium {
	class VIVIUM_API Animation {
	public:
		struct VIVIUM_API Data {
			std::vector<float> frame_timings; // Holds values for minimum time to be spent on each frame before displaying the next
			std::vector<int> frame_indices;   // Indices of sprites in texture atlas
			std::size_t frame_count = 0;

			Data(const std::vector<float>& frame_timings);
			Data(const std::string& animation_data_file, const Vector2<int>& atlas_dim = NULL);
		};
	private:
		Animation::Data m_AnimationData;

		std::shared_ptr<TextureAtlas> m_Atlas; // The texture atlas we got the sprites from

		Timer m_Timer;				// Timer to track elapsed time between updates
		float m_FrameTime = 0.0;	// Time spent displaying the current frame
		int m_FrameIndex = 0;		// Index of frame of the animation we're currently displaying

	public:
		static std::string FILE_EXTENSION;

		std::shared_ptr<Quad> quad;
		std::shared_ptr<Shader> shader;

		const std::shared_ptr<Texture> GetAtlas() const;

		// NOTE: data_filename refers to the filename of the .animation data file for the texture atlas
		Animation(std::shared_ptr<Quad> quad, std::shared_ptr<Shader> shader, std::shared_ptr<TextureAtlas> atlas, const Animation::Data& animation_data);
		~Animation();

		// Updates the animation, takes current time
		void Update();
	};
}
