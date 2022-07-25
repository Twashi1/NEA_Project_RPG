#pragma once

#include "Texture.h"
#include "Quad.h"
#include "Vector2.h"

// TODO: python scripting for animations? or python script to generate timings for animations?
// TODO: better names

namespace CoolEngineName {
	class ENGINE_API Animation {
	public:
		struct ENGINE_API Data {
			std::vector<float> frame_timings; // Holds values for minimum time to be spent on each frame before displaying the next
			int start_index = 0;			  // Index in texture atlas where we're meant to start the animation
			int frame_count;				  // Amount of sprites there are in the animation

			Data(const std::vector<float>& frame_timings);
			Data(const std::string& animation_data_file);
		};
	private:
		Animation::Data m_AnimationData;

		Vector2<int> m_SpriteSize; // Size of one sprite

		Vector2<int> m_AtlasDimRelative; // Dimensions of atlas in terms of sprites
		ENG_Ptr(Texture) m_Atlas;		 // The texture atlas we got the sprites from

		// Get index in texture atlas of the current frame
		Vector2<int> m_GetIndex();

		void m_Construct();

		Utils::Timer m_Timer;	 // Timer to track elapsed time between updates
		float m_FrameTime = 0.0; // Time spent displaying the current frame
		int m_FrameIndex = 0;	 // Index of frame of the animation we're currently displaying

	public:
		static std::string FILE_EXTENSION;

		ENG_Ptr(Quad) quad;
		ENG_Ptr(Shader) shader;

		const ENG_Ptr(Texture) GetAtlas() const;

		// NOTE: data_filename refers to the filename of the .animation data file for the texture atlas
		Animation(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader, ENG_Ptr(Texture) atlas, const Vector2<int>& sprite_size, const Animation::Data& animation_data);
		~Animation();

		// Updates the animation, takes current time
		void Update();
	};
}
