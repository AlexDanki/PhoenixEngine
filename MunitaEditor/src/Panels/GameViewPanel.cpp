#include "Panels/GameViewPanel.h"
#include "Core/App.h"

namespace ui = ImGui;

void GameViewPanel::Draw()
{
	
	UpdateRenderCamera();

	UpdateViewport();
	
	RenderToFrameBuffer();

	ui::Begin("GameView");

	ImVec2 size = ui::GetContentRegionAvail();
	m_currentSize = size;
	glm::vec2 currentView = glm::vec2{ size.x, size.y };

	if (currentView != m_viewportSize)
	{
		m_viewportSize = currentView;
		m_gameFramebuffer->Resize((uint32_t)currentView.x, (uint32_t)currentView.y);
		//m_app->SetViewport((uint32_t)currentView.x, (uint32_t)currentView.y);
	}

	ImGui::Image(
		(ImTextureID)(intptr_t)m_gameFramebuffer->GetColorAttachment(),
		size,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);
	ui::End();
}

void GameViewPanel::UpdateRenderCamera()
{
	if (m_renderCamera)
		m_app->SetRenderCamera(m_app->GetScene()->GetPrimaryCamera());
}

void GameViewPanel::UpdateViewport()
{
	if (m_currentSize.x != 0 && m_currentSize.y != 0)
	{
		m_app->SetViewport((uint32_t)m_currentSize.x, (uint32_t)m_currentSize.y);
	}
}


void GameViewPanel::RenderToFrameBuffer()
{
	m_gameFramebuffer->Bind();
	m_app->Render();
	m_gameFramebuffer->Unbind();
}