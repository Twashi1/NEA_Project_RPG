#pragma once

#include "Utils.h"
#include "Renderer.h"

#include <memory>
#include <vector>

class Shader;

class ENGINE_API ShaderManager {
public:
	static std::string projmat_name;

	static Utils::List<Shader*> shaders;

	static void UpdateProjectionMatrix(const glm::mat4& proj);
};