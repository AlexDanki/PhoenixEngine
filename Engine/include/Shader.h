#pragma once

using GLuint = unsigned int;

class Shader
{
public:
	Shader() = default;
	~Shader();

	bool Create(const char* vertShaderFile, const char* fragShaderFile);
	GLuint GetProgram() const;

	GLuint GetModelLocation() const;
	GLuint GetViewProjectionLocation() const;
	GLuint GetMaterialColorLocation() const;
	GLuint GetTextureLoc() const;

private:
	GLuint m_program = 0;

	GLuint m_modelLocation = -1;
	GLuint m_viewProjectionLocation = -1;
	GLuint m_materialColorLocation = -1;
	GLuint m_textureLocation = -1;

};