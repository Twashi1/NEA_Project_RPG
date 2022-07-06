#pragma once

#include "Utils.h"
#include "Renderer.h"
#include "Camera.h"

#include <memory>
#include <vector>

class Shader;

class ENGINE_API ShaderManager {
public:
	static std::string projmat_name;
	static std::string viewmat_name;

	static Utils::List<Shader*> shaders;

	static void UpdateShaders(const Camera& camera);
};