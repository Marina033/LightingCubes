#include "Shader.h"
#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


ShaderProgramSourse Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ss[0].str() = "";
    ss[1].str() = "";
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }

        }
        else {
            ss[(int)type] << line << '\n';
        }
    }
    if (ss[0].str() == "" || ss[1].str() == "") {
        std::cout << "problems with reading shader file" << std::endl;
    }
    return { ss[0].str(), ss[1].str() };
}
unsigned int Shader::CompileShader(unsigned int type, const std::string& sourse) {
    unsigned int id = glCreateShader(type);
    const char* src = sourse.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // char message[length]; error, because it is a stack
        // char* message = new char[length]; // heap, i don't like it
        char* message = (char*)_malloca(length * sizeof(char)); // _alloca instead of alloca
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex " : "fragment ") <<
            "shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;

    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0) {
    ShaderProgramSourse source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);


}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const { // bind shader - is use program
    auto q = m_RendererID;
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const 
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)  const
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) const
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) const
{
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformVec2f(const std::string& name, glm::vec2 vector) const
{
    GLCall(glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vector))); // fv - float array
}

void Shader::SetUniformVec3f(const std::string& name, glm::vec3 vector) const
{
    GLCall(glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vector))); // fv - float array
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4 matrix) const 
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix))); // fv - float array
}


int Shader::GetUniformLocation(const std::string& name) const {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name]; // optimization 
    }
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) { //ASSERT(location != -1);
        std::cout << "Warning uniform " << name << " doesn't exist" << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}