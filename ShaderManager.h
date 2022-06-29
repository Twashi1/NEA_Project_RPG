#pragma once

#include "Utilities.h"
#include "Renderer.h"

#include <memory>
#include <vector>

class Shader;

class ShaderManager {
	typedef std::vector<Shader*> ShaderList_t;

	static ShaderList_t m_Shaders;

public:
	static std::string projmat_name;

	static void AddShader(Shader* shader);
	static void RemoveShader(const Shader* shader);
	static void RemoveIndex(int index);

	static void UpdateProjectionMatrix(const glm::mat4& proj);
};