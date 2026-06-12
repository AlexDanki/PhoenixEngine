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

	m_modelLocation = glGetUniformLocation(m_program, "u_Model");
	m_viewProjectionLocation = glGetUniformLocation(m_program, "u_ViewProjection");
	m_materialColorLocation = glGetUniformLocation(m_program, "u_MaterialColor");
	m_textureLocation = glGetUniformLocation(m_program, "u_Texture");

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	return true;
}

GLuint Shader::GetProgram() const
{
	return m_program;
}

GLuint Shader::GetModelLocation() const
{
	return m_modelLocation;
}

GLuint Shader::GetViewProjectionLocation() const
{
	return m_viewProjectionLocation;
}

GLuint Shader::GetMaterialColorLocation() const
{
	return m_materialColorLocation;
}

GLuint Shader::GetTextureLoc() const
{
	return m_textureLocation;
}