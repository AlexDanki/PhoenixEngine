#include "glad/glad.h"
#include "Shader.h" 
#include <iostream>

Shader::~Shader()
{
	if(m_program != 0)
	{
		glDeleteProgram(m_program);
	}
}

bool Shader::Create(const char* vertShaderFile, const char* fragShaderFile)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource( vertexShader, 1, &vertShaderFile, nullptr);
	glCompileShader(vertexShader);

	int sucess;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &sucess);
	if(!sucess)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);

		std::cout << "Erro ao compilar VertexShader - " << infoLog << std::endl;
		return false;
	}

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderFile, nullptr);
	glCompileShader(fragShader);

	sucess;
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &sucess);
	if (!sucess)
	{
		char infoLog[512];
		glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
		std::cout << "Erro ao compilar FragmentShader - " << infoLog << std::endl;
		return false;
	}

	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragShader);
	glLinkProgram(m_program);

	sucess;
	glGetShaderiv(m_program, GL_LINK_STATUS, &sucess);
	if (!sucess)
	{
		char infoLog[512];
		glGetProgramInfoLog(m_program, 512, nullptr, infoLog);
		std::cout << "Erro ao Linkar programa - " << infoLog << std::endl;
		return false;
	}

	// Procura todos os uniforms nos shaders
	SearchAllUniformLocations();

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	return true;
}

GLuint Shader::GetProgram() const { return m_program; }

void Shader::Use()
{
	glUseProgram(m_program);
}

// Procura todos os uniforms nos shaders
void Shader::SearchAllUniformLocations()
{
	// Camera Location
	m_cameraPositionLocation = GetUniformLocation("u_CameraPosition");

	// Model Location
	m_modelLocation = GetUniformLocation("u_Model");

	// Camera Location
	m_viewProjectionLocation = GetUniformLocation("u_ViewProjection");

	// Material Locations
	m_materialColorLocation = GetUniformLocation("u_MaterialColor");
	m_textureLocation = GetUniformLocation("u_Texture");
	m_colorLocation = GetUniformLocation("u_Color");

	// Enviromments Locations
	m_directionalLightLoc = GetUniformLocation("u_sun.Direction");
	m_directionalLightColorLoc = GetUniformLocation("u_sun.Color");
	m_directionalLightIntensityLoc = GetUniformLocation("u_sun.Intensity");
	m_ambinetColorLoc = GetUniformLocation("u_AmbienteColor");
	m_ambinetIntensityLoc = GetUniformLocation("u_AmbienteIntensity");
	
	m_fogEnabledLoc = GetUniformLocation("u_Fog.Enabled");
	m_fogColorLoc = GetUniformLocation("u_Fog.Color");
	m_fogDensityLoc = GetUniformLocation("u_Fog.Density");
}

// Pega uniforms no shader do programa
GLuint Shader::GetUniformLocation(const std::string& name)
{
	return glGetUniformLocation(m_program, name.c_str());
}



// FUNÇÕES PARA PEGAR AS LOCALIZAÇÕES DOS UNIFORMS NOS SHADERS
// Camera Location
GLuint Shader::GetCameraPositionLocation() const { return m_cameraPositionLocation; }

// Model Location
GLuint Shader::GetModelLocation() const { return m_modelLocation; }

// Camera Location
GLuint Shader::GetViewProjectionLocation() const { return m_viewProjectionLocation; }

// Material Locations
GLuint Shader::GetMaterialColorLocation() const { return m_materialColorLocation; }
GLuint Shader::GetTextureLoc() const { return m_textureLocation; }
GLuint Shader::GetColorLocation() const { return m_colorLocation; }

// Enviromments Locations
GLuint Shader::GetDirectionalLightLocation() const { return m_directionalLightLoc; }	// Direactional Lights
GLuint Shader::GetDirectionalLightColorLocation() const { return m_directionalLightColorLoc; }
GLuint Shader::GetDirectionalLightIntensityLocation() const { return m_directionalLightIntensityLoc; }

GLuint Shader::GetAmbineteColorLocation() const { return m_ambinetColorLoc; }			// Ambinete Lights
GLuint Shader::GetAmbineteIntensityLocation() const { return m_ambinetIntensityLoc; }

GLuint Shader::GetFogEnabledLocation() const { return m_fogEnabledLoc; }				// Fog
GLuint Shader::GetFogColorLocation() const { return m_fogColorLoc; }				
GLuint Shader::GetFogDensityLocation() const { return m_fogDensityLoc; }			