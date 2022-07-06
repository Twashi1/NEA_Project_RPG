#include "ShaderManager.h"
#include "Shader.h"

Utils::List<Shader*> ShaderManager::shaders = Utils::List<Shader*>();
std::string ShaderManager::projmat_name = "u_projMat";

void ShaderManager::UpdateShaders(const Camera& camera)
{
	/*
	for (Shader* shader : shaders) {
		shader->Bind();
		shader->SetUniformMat4fv(projmat_name.c_str(), camera.GetProjMat());
	}

	Shader::Unbind();
	*/
}

void ShaderManager::UpdateProjectionMatrix(const glm::mat4& proj)
{
	for (Shader* shader : shaders) {
		shader->Bind();
		shader->SetUniformMat4fv(projmat_name.c_str(), proj);
	}

	Shader::Unbind();
}
