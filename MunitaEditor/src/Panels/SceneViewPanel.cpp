#include "Panels/SceneViewPanel.h"
#include "Core/App.h"

void SceneViewPanel::Draw()
{
	
	UpdateRenderCamera();

	UpdateViewport();

	RenderToFrameBuffer();

	ui::Begin("SceneView");

	ImVec2 size = ui::GetContentRegionAvail();
	m_currentSize = size;
	glm::vec2 currentView = glm::vec2{ m_currentSize.x, m_currentSize.y };
	if (currentView != m_viewportSize)
	{
		m_viewportSize = currentView;
		m_sceneFramebuffer->Resize((uint32_t)currentView.x, (uint32_t)currentView.y);
		
	}

	ImGui::Image(
		(ImTextureID)(intptr_t)m_sceneFramebuffer->GetColorAttachment(),
		size,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);
	ui::End();
}



void SceneViewPanel::UpdateRenderCamera()
{
	if (m_renderCamera)
		m_app->SetRenderCamera(m_renderCamera);
}

void SceneViewPanel::UpdateViewport()
{
	if (m_currentSize.x != 0 && m_currentSize.y != 0)
		m_app->SetViewport((uint32_t)m_currentSize.x, (uint32_t)m_currentSize.y);
}

void SceneViewPanel::RenderToFrameBuffer()
{
	m_sceneFramebuffer->Bind();
	m_app->Render();
	m_sceneFramebuffer->Unbind();
}