#include "ShaderManager.h"
#include "Shader.h"

std::vector<Shader*> ShaderManager::shaders{};
std::string ShaderManager::projmat_name = "u_projMat";
std::string ShaderManager::viewmat_name = "u_viewMat";
std::string ShaderManager::offset_name = "u_Offset";

void ShaderManager::UpdateShaders(const ENG_Ptr(Camera) camera)
{
	for (Shader* shader : shaders) {
		shader->Bind();
		shader->SetUniformMat4fv(projmat_name.c_str(), camera->GetProjMat());
		shader->SetUniformMat4fv(viewmat_name.c_str(), camera->GetViewMat());
	}

	Shader::Unbind();
}
