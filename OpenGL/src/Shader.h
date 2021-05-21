#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"



struct ShaderProgramSourse {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
	// это поле может изменяться константной функцией GetUniformLocation 
	// caching for uniforms
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//set uniforms
	void SetUniform1i(const std::string& name, int value) const;
	void SetUniform1f(const std::string& name, float value) const;
	void SetUniform3f(const std::string& name, float v0, float v1, float v2) const;
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
	void SetUniformVec2f(const std::string& name, glm::vec2 vector) const;
	void SetUniformVec3f(const std::string& name, glm::vec3 vector) const;
	void SetUniformMat4f(const std::string& name, glm::mat4 matrix) const;
private:
	ShaderProgramSourse ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& sourse);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name) const;

};


