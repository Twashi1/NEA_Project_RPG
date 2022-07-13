#pragma once

#include "Utils.h"
#include "Renderer.h"
#include "Camera.h"

#include <memory>
#include <vector>

// TODO: add some default shaders

class Shader;

class ENGINE_API ShaderManager {
public:
	static std::string projmat_name;
	static std::string offset_name;

	static std::vector<Shader*> shaders;

	static void UpdateShaders(const Camera& camera);
	static void UpdateProjectionMatrix(const glm::mat4& proj);
};