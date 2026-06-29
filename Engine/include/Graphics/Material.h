#pragma once
#include <glm/glm.hpp>

class Texture;
class Shader;


class Material
{
public:
	Material(Shader* shader);
	void SetTexture(Texture* texture);
	Texture* GetTexture() const;

	void SetShader(Shader* shader) { m_shader = shader; };
	Shader* GetShader() const;

	glm::vec3 GetMaterialColor() const;
	glm::vec3& GetMaterialColor();
	void SetMaterialColor(const glm::vec3& color);

private:
	Texture* m_texture = nullptr;
	Shader* m_shader = nullptr;
	glm::vec3 m_color = { 1.0, 1.0, 1.0 };
};