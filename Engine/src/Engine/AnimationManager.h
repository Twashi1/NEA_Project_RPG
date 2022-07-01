#pragma once

#include "Utilities.h"

class Animation;

class ENGINE_API AnimationManager {
public:
	static Utils::List<Animation*> animations;

	static void Update(float current_time);

	friend Animation;
};