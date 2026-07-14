#include "Panels/SceneViewPanel.h"

void SceneViewPanel::Draw()
{
	ui::Begin("SceneView");
	ImVec2 size = ui::GetContentRegionAvail();
	ImGui::Image(
		(ImTextureID)(intptr_t)m_sceneFramebuffer->GetColorAttachment(),
		size,
		ImVec2(0, 1),
		ImVec2(1, 0)
	);
	ui::End();
}