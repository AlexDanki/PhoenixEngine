#include "Panels/EditorPanel.h"
#include "Graphics/Framebuffer.h"

class SceneViewPanel : public EditorPanel
{
public:
	void Draw() override;
	void SetFramebuffer(Mse::Framebuffer* framebuffer){ m_sceneFramebuffer = framebuffer; }

private:
	Mse::Framebuffer* m_sceneFramebuffer;
};