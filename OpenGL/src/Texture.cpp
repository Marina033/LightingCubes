#include "Texture.h"
#include "vendor/stb_image/stb_image.h"

#include <iostream>

Texture::Texture(const std::string& path, const unsigned int flagWrap) :
		m_FilePath(path), m_LocalBuffer(nullptr),
		m_Width(0), m_Height(0), m_BBP(0), m_FlagWrap(flagWrap) {
	// строчка чтобы котечка смотрела неперевернуто
	stbi_set_flip_vertically_on_load(1); //вместе с этой строчкой jpg не работает, но мб работает

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_FlagWrap)); // 
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_FlagWrap));


	GLCall(m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BBP, 4));
	if (m_LocalBuffer) {
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR of loading texture in texture.cpp" << std::endl;
	}
	//GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // unbind

	if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer);
	}
}

Texture::~Texture() {
	GLCall(glDeleteTextures(1, &m_RendererID));

}

void Texture::Bind(unsigned int slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

}

void Texture::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

}
