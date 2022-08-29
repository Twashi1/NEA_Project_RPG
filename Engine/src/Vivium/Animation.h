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
			int start_index = 0;			  // Index in texture atlas where we're meant to start the animation
			int frame_count;				  // Amount of sprites there are in the animation

			Data(const std::vector<float>& frame_timings);
			Data(const std::string& animation_data_file);
		};
	private:
		Animation::Data m_AnimationData;

		Ref(TextureAtlas) m_Atlas; // The texture atlas we got the sprites from

		Timer m_Timer;	 // Timer to track elapsed time between updates
		float m_FrameTime = 0.0; // Time spent displaying the current frame
		int m_FrameIndex = 0;	 // Index of frame of the animation we're currently displaying

	public:
		static std::string FILE_EXTENSION;

		Ref(Quad) quad;
		Ref(Shader) shader;

		const Ref(Texture) GetAtlas() const;

		// NOTE: data_filename refers to the filename of the .animation data file for the texture atlas
		Animation(Ref(Quad) quad, Ref(Shader) shader, Ref(TextureAtlas) atlas, const Animation::Data& animation_data);
		~Animation();

		// Updates the animation, takes current time
		void Update();
	};
}
