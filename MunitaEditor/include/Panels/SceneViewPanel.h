#include "Panels/EditorPanel.h"
#include "Graphics/Framebuffer.h"
#include <glm/glm.hpp>

class ICamera;

class App;

class SceneViewPanel : public EditorPanel
{
public:
	void Draw() override;
	void SetFramebuffer(Mse::Framebuffer* framebuffer){ m_sceneFramebuffer = framebuffer; }
	void SetApp(App* app){ m_app = app; };
	void SetRenderCamera(ICamera* renderCamera) { m_renderCamera = renderCamera; };

	void UpdateRenderCamera();
	void UpdateViewport();
	void RenderToFrameBuffer();

private:
	Mse::Framebuffer* m_sceneFramebuffer;
	glm::vec2 m_viewportSize = glm::vec2{0,0};
	App* m_app;
	ICamera* m_renderCamera = nullptr;
	ImVec2 m_currentSize;
};