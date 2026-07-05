#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Component.h"

class Font;

class WorldTextRenderComponent : public Component
{
public:
	void SetText(const std::string& text);
	const std::string& GetText() const ;

	void SetFont(Font* font);
	const Font* GetFont() const;

	void SetScale(float scale);
	const float GetScale() const;

private:
	std::string m_text = "Munita";
	Font* m_font = nullptr;
	float m_scale = 0.03f;
};