#include "Shader.h"

namespace CoolEngineName {
    std::string Shader::PATH = "";
    std::string Shader::EXTENSION = "";

    GLuint Shader::CompileShader(const std::string& source, unsigned int type)
    {
        // Create shader
        GlCall(GLuint id = glCreateShader(type));
        // Pass text into shader
        const char* src = source.c_str();
        GlCall(glShaderSource(id, 1, &src, nullptr));
        // Compile shader
        GlCall(glCompileShader(id));

        GLint result;
        GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

        // Check for compilation errors
        if (result == GL_FALSE) {
            GLint length;
            GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
            // Create buffer to store compilation error in
            char* message = new char[length];

            GlCall(glGetShaderInfoLog(id, length, &length, message));

            // Check on which shader the error occured
            // TODO: use Log
            const char* shader_type;
            shader_type = (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
            std::cout
                << shader_type
                << "Shader compilation failed: "
                << message << std::endl;

            delete[] message;

            // Delete shader since compilation failed
            GlCall(glDeleteShader(id));

            return 0;
        }

        return id;
    }

    GLuint Shader::CreateShader(const char* vertex_name, const char* frag_name) {
        GLuint program = glCreateProgram();

        // Read files and return pointer to string
        std::string* vertex_shader = Utils::ReadFile(vertex_name);
        std::string* fragment_shader = Utils::ReadFile(frag_name);

        // Compile the shaders
        GLuint vs = CompileShader(*vertex_shader, GL_VERTEX_SHADER);
        GLuint fs = CompileShader(*fragment_shader, GL_FRAGMENT_SHADER);

        // Delete the text
        delete vertex_shader;
        delete fragment_shader;

        // Attach vertex and fragment shader to program
        GlCall(glAttachShader(program, vs));
        GlCall(glAttachShader(program, fs));
        GlCall(glLinkProgram(program));
        // Compile program
        GlCall(glValidateProgram(program));

        return program;
    }

    Shader::Shader(const char* vs_file, const char* fs_file)
    {
        id = CreateShader(
            (PATH + std::string(vs_file) + EXTENSION).c_str(),
            (PATH + std::string(fs_file) + EXTENSION).c_str()
        );
        Bind();
    }

    Shader::~Shader()
    {
        // Delete program
        GlCall(glDeleteProgram(id));
    }

    void Shader::Unbind()
    {
        GlCall(glUseProgram(NULL));
    }

    void Shader::Bind() const
    {
        GlCall(glUseProgram(id));
    }

    void Shader::SetUniform1i(const char* name, int x)
    {
        GlCall(GLuint location = glGetUniformLocation(id, name));
        GlCall(glUniform1i(location, x));
    }

    void Shader::SetUniform1ui(const char* name, unsigned int x)
    {
        GlCall(GLuint location = glGetUniformLocation(id, name));
        GlCall(glUniform1ui(location, x));
    }

    void Shader::SetUniform1f(const char* name, float x)
    {
        GlCall(GLuint location = glGetUniformLocation(id, name));
        GlCall(glUniform1f(location, x));
    }

    void Shader::SetUniform2f(const char* name, float x, float y)
    {
        GlCall(GLuint location = glGetUniformLocation(id, name));
        GlCall(glUniform2f(location, x, y));
    }

    void Shader::SetUniform2f(const char* name, Vector2<float> v)
    {
        GlCall(GLuint location = glGetUniformLocation(id, name));
        GlCall(glUniform2f(location, v.x, v.y));
    }

    void Shader::SetUniform3f(const char* name, float x, float y, float z)
    {
        GlCall(GLuint location = glGetUniformLocation(id, name));
        GlCall(glUniform3f(location, x, y, z));
    }

    void Shader::SetUniform3f(const char* name, Vector3<float> v)
    {
        GlCall(GLuint location = glGetUniformLocation(id, name));
        GlCall(glUniform3f(location, v.x, v.y, v.z));
    }

    void Shader::SetUniform4f(const char* name, float x, float y, float z, float w)
    {
        GlCall(GLuint location = glGetUniformLocation(id, name));
        GlCall(glUniform4f(location, x, y, z, w));
    }

    void Shader::SetUniformMat4fv(const char* name, const glm::mat4& mat)
    {
        GlCall(GLuint location = glGetUniformLocation(id, name));
        GlCall(glUniformMatrix4fv(location, 1, false, &mat[0][0]));
    }

    void Shader::SetUniform1fv(const char* name, unsigned int count, const float* values)
    {
        GlCall(GLuint location = glGetUniformLocation(id, name));
        GlCall(glUniform1fv(location, count, values));
    }

    void Shader::SetUniform1uiv(const char* name, unsigned int count, const unsigned int* values)
    {
        GlCall(GLuint location = glGetUniformLocation(id, name));
        GlCall(glUniform1uiv(location, count, values));
    }
}