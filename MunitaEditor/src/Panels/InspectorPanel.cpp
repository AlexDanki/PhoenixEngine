#include "Panels/InspectorPanel.h"
//#include <imgui.h>

void InspectorPanel::Draw()
{
	ui::Begin("Inspector");
	ui::Text("InspectorPanel");
	ui::End();
}