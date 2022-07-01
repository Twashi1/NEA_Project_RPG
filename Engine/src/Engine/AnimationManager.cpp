#include "AnimationManager.h"
#include "Animation.h"

Utils::List<Animation*> AnimationManager::animations = {};

void AnimationManager::Update(float current_time)
{
	for (Animation* animation : animations) {
		animation->Update(current_time);
	}
}
