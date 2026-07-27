#include "Framebuffer.h"
#include <iostream>
#include <glad/glad.h>

Mse::Framebuffer::Framebuffer(int width, int height)
	:m_width(width),
	m_height(height)
{
	// Crie o Framebuffer
	glGenFramebuffers(1, &m_id);

	// Cria a textura
	glGenTextures(1, &m_colorAttachment);

	FrameBufferConfig();
}

Mse::Framebuffer::~Framebuffer()
{
	if(m_id != 0)
		glDeleteFramebuffers(1, &m_id);

	if (m_colorAttachment != 0)
		glDeleteTextures(1, &m_colorAttachment);
}

void Mse::Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	glViewport(0, 0, m_width, m_height);
}

void Mse::Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);
}

void Mse::Framebuffer::Resize(int width, int height)
{
	m_width = width;
	m_height = height;

	if (m_width == 0 || m_height == 0) return;

	FrameBufferConfig();
}

void Mse::Framebuffer::FrameBufferConfig()
{
	// Liga o Framebuffer
	Bind();

	// Liga a textura
	glBindTexture(GL_TEXTURE_2D, m_colorAttachment);

	// Configura a textura
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Desliga a textura
	glBindTexture(GL_TEXTURE_2D, 0);

	// Conecta framebuffer e textura
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

	// Verifica erro
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer incompleto!\n";
	}

	// Desliga o frame buffer
	Unbind();
}