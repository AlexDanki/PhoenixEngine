#include "WorldTextRenderComponent.h"
#include "Font.h"

void WorldTextRenderComponent::Serialize(std::ofstream& file) const
{
	file << "COMPONENT\n";

	file << "WORLD_TEXT\n";

	// texto
	file << "TEXT\n";
	file << m_text << "\n";
	file << "END_TEXT\n";

	// scale
	file << "SCALE\n";
	file << m_scale << "\n";

	file << "END_COMPONENT\n";
}

void WorldTextRenderComponent::SetText(const std::string& text)
{
	m_text = text;
}

const std::string& WorldTextRenderComponent::GetText() const
{
	return m_text;
}

void WorldTextRenderComponent::SetFont(Font* font)
{
	m_font = font;
}

const Font* WorldTextRenderComponent::GetFont() const
{
	return m_font;
}

void WorldTextRenderComponent::SetScale(float scale)
{
	m_scale = scale;
}

const float WorldTextRenderComponent::GetScale() const
{
	return m_scale;
}