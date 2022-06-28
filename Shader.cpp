#include "Shader.h"

std::string Shader::PATH = "";
std::string Shader::EXTENSION = "";

std::string* Shader::ReadFile(const std::string& path)
{
    std::ifstream file(path, std::ios::in); // Create file pointer
    std::string* out = new std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); // Read all text and store on heap

    return out; // Return pointer to text
}

GLuint Shader::CompileShader(const std::string& source, unsigned int type)
{
    GlCall(GLuint id = glCreateShader(type));
    const char* src = source.c_str();
    GlCall(glShaderSource(id, 1, &src, nullptr));
    GlCall(glCompileShader(id));

    GLint result;
    GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE) {
        GLint length;
        GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = new char[length];

        GlCall(glGetShaderInfoLog(id, length, &length, message));

        const char* s_type;
        s_type = (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
        std::cout
            << s_type
            << "Shader compilation failed: "
            << message << std::endl;

        delete[] message;

        GlCall(glDeleteShader(id));

        return 0;
    }

    return id;
}

GLuint Shader::CreateShader(const char* vertex_name, const char* frag_name) {
    GLuint program = glCreateProgram();

    // Read files and return pointer to string (which is on heap)
    std::string* vertexShader = ReadFile(vertex_name);
    std::string* fragmentShader = ReadFile(frag_name);

    // Compile the shaders
    GLuint vs = CompileShader(*vertexShader, GL_VERTEX_SHADER);
    GLuint fs = CompileShader(*fragmentShader, GL_FRAGMENT_SHADER);

    // Delete the text
    delete vertexShader;
    delete fragmentShader;

    GlCall(glAttachShader(program, vs));
    GlCall(glAttachShader(program, fs));
    GlCall(glLinkProgram(program));
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

    // Add ourselves to the shader manager
    ShaderManager::AddShader(this);
}

Shader::~Shader()
{
    // Remove ourselves from shader manager
    ShaderManager::RemoveShader(this);

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
