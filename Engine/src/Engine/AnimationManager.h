#pragma once

#include "Utils.h"

class Animation;

class ENGINE_API AnimationManager {
public:
	static std::vector<Animation*> animations;

	static void Update(float current_time);

	friend Animation;
};