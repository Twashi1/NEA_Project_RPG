#include "ShaderManager.h"
#include "Shader.h"

ShaderManager::ShaderList_t ShaderManager::m_shaders = ShaderList_t();
std::string ShaderManager::projmat_name = "u_projMat";

void ShaderManager::AddShader(Shader* shader)
{
	m_shaders.push_back(shader);
}

void ShaderManager::RemoveShader(const Shader* shader)
{
	// Remove all occurences of shader from m_shaders, and return new end of list
	auto new_end = std::remove(m_shaders.begin(), m_shaders.end(), shader);
	
	// Erase all elements from the new end of the list to the old end of the list
	m_shaders.erase(new_end, m_shaders.end());
}

void ShaderManager::RemoveIndex(int index)
{
	if (index < m_shaders.size()) {
		m_shaders.erase(m_shaders.begin() + index);
	}
	else {
		std::stringstream ss; ss << "Index out of bounds, " << index << " > " << m_shaders.size();
		Log(ss.str(), Utilities::ERROR::FATAL);
	}
}

void ShaderManager::UpdateProjectionMatrix(const glm::mat4& proj)
{
	for (Shader* shader : m_shaders) {
		shader->Bind();
		shader->SetUniformMat4fv(projmat_name.c_str(), proj);
	}

	Shader::Unbind();
}
