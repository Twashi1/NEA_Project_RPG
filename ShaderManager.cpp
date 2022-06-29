#include "ShaderManager.h"
#include "Shader.h"

ShaderManager::ShaderList_t ShaderManager::m_Shaders = ShaderList_t();
std::string ShaderManager::projmat_name = "u_projMat";

void ShaderManager::AddShader(Shader* shader)
{
	m_Shaders.push_back(shader);
}

void ShaderManager::RemoveShader(const Shader* shader)
{
	// Remove all occurences of shader from m_Shaders, and return new end of list
	auto new_end = std::remove(m_Shaders.begin(), m_Shaders.end(), shader);
	
	// Erase all elements from the new end of the list to the old end of the list
	m_Shaders.erase(new_end, m_Shaders.end());
}

void ShaderManager::RemoveIndex(int index)
{
	if (index < m_Shaders.size()) {
		m_Shaders.erase(m_Shaders.begin() + index);
	}
	else {
		std::string text = std::format("Index out of bounds: {} > {}", to_string(index), to_string(m_Shaders.size()));
		Log(text, Utilities::ERROR::FATAL);
	}
}

void ShaderManager::UpdateProjectionMatrix(const glm::mat4& proj)
{
	for (Shader* shader : m_Shaders) {
		shader->Bind();
		shader->SetUniformMat4fv(projmat_name.c_str(), proj);
	}

	Shader::Unbind();
}
