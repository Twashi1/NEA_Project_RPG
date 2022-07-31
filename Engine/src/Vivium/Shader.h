#pragma once

#include "Core.h"
#include "Logger.h"
#include "Math.h"
#include "Utils.h"

namespace Vivium {
    class VIVIUM_API Shader {
    private:
        // Registers shader with opengl
        GLuint m_CompileShader(const std::string& source, unsigned int type);
        // Compiles vertex and fragment shader
        GLuint m_CreateShader(const char* vertex_name, const char* frag_name);

    public:
        GLuint id;

        Shader(const char* vs_file, const char* fs_file);
        ~Shader();

        static void Unbind();
        void Bind() const;

        // Uniform setters
        void SetUniform1i(const char* name, int x);
        void SetUniform1ui(const char* name, unsigned int x);
        void SetUniform1f(const char* name, float x);
        void SetUniform2f(const char* name, float x, float y);
        void SetUniform2f(const char* name, Vector2<float> v);
        void SetUniform3f(const char* name, float x, float y, float z);
        void SetUniform4f(const char* name, float x, float y, float z, float w);
        void SetUniform1fv(const char* name, unsigned int count, const float* values);
        void SetUniform1uiv(const char* name, unsigned int count, const unsigned int* values);
        void SetUniformMat4fv(const char* name, const glm::mat4& mat);
    };
}
