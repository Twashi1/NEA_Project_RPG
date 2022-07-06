#include "ShaderManager.h"
#include "Shader.h"

Utils::List<Shader*> ShaderManager::shaders = Utils::List<Shader*>();
std::string ShaderManager::projmat_name = "u_projMat";
std::string ShaderManager::viewmat_name = "u_viewMat";

void ShaderManager::UpdateShaders(const Camera& camera)
{
	for (Shader* shader : shaders) {
		shader->Bind();
		shader->SetUniformMat4fv(projmat_name.c_str(), camera.GetProjMat());
		shader->SetUniformMat4fv(viewmat_name.c_str(), camera.GetViewMat());
	}

	Shader::Unbind();
}
