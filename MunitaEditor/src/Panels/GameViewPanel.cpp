#include "Panels/GameViewPanel.h"

namespace ui = ImGui;

void GameViewPanel::Draw()
{
	ui::Begin("GameView");
	ui::Text("GameViewPanel");
	ui::End();
}