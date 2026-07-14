#pragma once

namespace Mse
{
	class Framebuffer
	{
	public:
		Framebuffer(int width, int height);
		~Framebuffer();

		void Bind();
		void Unbind();

		int GetWidth() { return m_width; }
		int GetHeight() { return m_height; }

		unsigned int GetColorAttachment() const { return m_colorAttachment; }

	private:
		unsigned int m_id = 0;
		unsigned int m_colorAttachment = 0;

		int m_width = 1280;
		int m_height = 720;
	};
}