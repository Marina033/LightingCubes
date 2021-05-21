#pragma once
#include "Renderer.h"
class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BBP;
	unsigned int m_FlagWrap;
public:
	Texture(const std::string& path, const unsigned int flag = GL_CLAMP_TO_EDGE);
	~Texture();


	void Bind(unsigned int slot = 0) const;
	void Unbind() const;


	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }
};


