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

	// Camera Location
	GLuint GetCameraPositionLocation() const;

	// Model Location
	GLuint GetModelLocation() const;

	// Camera Location
	GLuint GetViewProjectionLocation() const;

	// Material Locations
	GLuint GetMaterialColorLocation() const;
	GLuint GetTextureLoc() const;
	GLuint GetColorLocation() const;

	// Enviromments Locations
	GLuint GetDirectionalLightLocation() const;			// Directional Light
	GLuint GetDirectionalLightColorLocation() const;
	GLuint GetDirectionalLightIntensityLocation() const;

	GLuint GetAmbineteColorLocation() const;			// Ambiente Light
	GLuint GetAmbineteIntensityLocation() const;

	GLuint GetFogEnabledLocation() const;				// Fog
	GLuint GetFogColorLocation() const;					
	GLuint GetFogDensityLocation() const;					

	// Procura todos os uniforms nos shaders
	void SearchAllUniformLocations();

	GLuint Shader::GetUniformLocation(const std::string& name);

private:
	GLuint m_program = 0;
	
	// Camera Location
	GLuint m_cameraPositionLocation = -1;

	// Model Location
	GLuint m_modelLocation = -1;

	// Camera Locations
	GLuint m_viewProjectionLocation = -1;

	// Material Locations
	GLuint m_materialColorLocation = -1;
	GLuint m_textureLocation = -1;
	GLuint m_colorLocation;

	// Enviromments Locations
	GLuint m_directionalLightLoc = -1;
	GLuint m_directionalLightColorLoc = -1;
	GLuint m_directionalLightIntensityLoc = -1;
	GLuint m_ambinetColorLoc = -1;
	GLuint m_ambinetIntensityLoc = -1;

	// FOG
	GLuint m_fogEnabledLoc = -1;
	GLuint m_fogColorLoc = -1;
	GLuint m_fogDensityLoc = -1;



};