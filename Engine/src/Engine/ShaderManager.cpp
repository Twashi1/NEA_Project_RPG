#include "ShaderManager.h"
#include "Shader.h"

Utils::List<Shader*> ShaderManager::shaders = Utils::List<Shader*>();
std::string ShaderManager::projmat_name = "u_projMat";
std::string ShaderManager::offset_name = "u_Offset";

void ShaderManager::UpdateShaders(const Camera& camera)
{
	for (Shader* shader : shaders) {
		shader->Bind();
		shader->SetUniform2f(offset_name.c_str(), -camera.pos);
	}

	Shader::Unbind();
}

void ShaderManager::UpdateProjectionMatrix(const glm::mat4& proj)
{
	for (Shader* shader : shaders) {
		shader->Bind();
		shader->SetUniformMat4fv(projmat_name.c_str(), proj);
	}

	Shader::Unbind();
}
