#include "AnimationManager.h"
#include "Animation.h"

std::vector<Animation*> AnimationManager::animations = {};

void AnimationManager::Update(float current_time)
{
	for (Animation* animation : animations) {
		animation->Update(current_time);
	}
}
