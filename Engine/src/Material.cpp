#include "Material.h"

Material::Material(Shader* shader)
	:m_shader(shader)
{}

void Material::SetTexture(Texture* texture)
{
	m_texture = texture;
}

Texture* Material::GetTexture() const
{
	return m_texture;
}

Shader* Material::GetShader() const
{
	return m_shader;
}

glm::vec3 Material::GetMaterialColor() const
{
	return m_color;
}

void Material::SetMaterialColor(const glm::vec3& color)
{
	m_color = color;
}