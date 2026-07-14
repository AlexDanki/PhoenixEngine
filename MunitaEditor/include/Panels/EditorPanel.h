#pragma once
#include <imgui.h>

namespace ui = ImGui;

class EditorPanel
{
	
public:
	virtual ~EditorPanel() = default;
	
	virtual void Draw() = 0;

private:
	
};