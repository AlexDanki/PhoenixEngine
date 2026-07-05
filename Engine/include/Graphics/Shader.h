#pragma once
#include <string>

using GLuint = unsigned int;

class Shader
{
public:
	Shader() = default;
	~Shader();
	

	bool Create(const char* vertShaderFile, const char* fragShaderFile);
	GLuint GetProgram() const;
	void Use();

	GLuint GetModelLocation() const;
	GLuint GetViewProjectionLocation() const;
	GLuint GetMaterialColorLocation() const;
	GLuint GetTextureLoc() const;
	GLuint GetColorLocation() const;

	GLuint GetDirectionalLightLocation() const;
	GLuint GetDirectionalLightColorLocation() const;
	GLuint GetDirectionalLightIntensityLocation() const;
	GLuint GetAmbineteColorLocation() const;
	GLuint GetAmbineteIntensityLocation() const;

	GLuint Shader::GetUniformLocation(const std::string& name);

private:
	GLuint m_program = 0;

	GLuint m_modelLocation = -1;
	GLuint m_viewProjectionLocation = -1;
	GLuint m_materialColorLocation = -1;
	GLuint m_textureLocation = -1;
	GLuint m_colorLocation;

	GLuint m_directionalLightLoc = -1;
	GLuint m_directionalLightColorLoc = -1;
	GLuint m_directionalLightIntensityLoc = -1;
	GLuint m_ambinetColorLoc = -1;
	GLuint m_ambinetIntensityLoc = -1;


};