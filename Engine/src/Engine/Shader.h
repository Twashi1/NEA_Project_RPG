#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "ShaderManager.h"
#include "Utilities.h"
#include "Vector3.h"
#include "Vector2.h"

// TODO: creating a shader will look in the Sandbox/res/shaders directory, however some shaders are used within Engine,
// thus should have their resource files here as well

class ENGINE_API Shader {
private:
    // Registers shader with opengl
    GLuint CompileShader(const std::string& source, unsigned int type);
    // Compiles vertex and fragment shader
    GLuint CreateShader(const char* vertex_name, const char* frag_name);

public:
    static std::string PATH;      // Path to shader
    static std::string EXTENSION; // Shader file extension

    Shader(const char* vs_file, const char* fs_file);
    ~Shader();

    static void Unbind();
    void Bind() const;

    // Uniform setters
    void SetUniform1i    (const char* name, int x);
    void SetUniform1ui   (const char* name, unsigned int x);
    void SetUniform1f    (const char* name, float x);
    void SetUniform2f    (const char* name, float x, float y);
    void SetUniform2f    (const char* name, Vector2<float> v);
    void SetUniform3f    (const char* name, float x, float y, float z);
    void SetUniform3f    (const char* name, Vector3<float> v);
    void SetUniform4f    (const char* name, float x, float y, float z, float w);
    void SetUniform1fv   (const char* name, unsigned int count, const float* values);
    void SetUniform1uiv  (const char* name, unsigned int count, const unsigned int* values);
    void SetUniformMat4fv(const char* name, const glm::mat4& mat);

    GLuint id;
};