#include "GUI.h"

#include <Engine/Config.h>
#include <Engine/GUI.h>
#include "Cursor.h"
#include "Engine/IMGUIInclude.h"

GUI::GUI() :Engine::GUI()
{
	Cursor::Init();
}

void GUI::Draw()
{
	Engine::GUI::Draw();
	drawDebugMenu();
}

void GUI::drawDebugMenu()
{
#if !defined(NO_IMGUI)
	if (cheatSheetOpen)
	{
		ImGui::SetNextWindowSize(ImVec2(300, 300));

		//To make minimized at start
		//ImGui::SetNextWindowCollapsed(true);

		// Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Begin("Debug Menu", &Engine::Config::debugMenuEnabled,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

		ImGui::End();
	}

#endif
}