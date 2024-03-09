#include "GUI.h"
#include <string>

#if !defined(__SWITCH__) && !defined(__APPLE__)
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#endif

#include "Globals.h"
#include <Engine/Config.h>
#include "Loader.h"
#include <Engine/GUI.h>
#include "Cursor.h"

GUI::GUI() :Engine::GUI()
{
	Cursor::Init();
}

void GUI::Draw()
{
	Engine::GUI::Draw();
	drawDebugMenu();
}

#if !defined(__SWITCH__) && !defined(__APPLE__)
void GUI::drawDebugMenu()
{
	if (debugMenuOpen)
	{
		ImGui::SetNextWindowSize(ImVec2(700, 500));

		//To make minimized at start
		//ImGui::SetNextWindowCollapsed(true);

		// Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Begin("Debug Menu", &debugMenuOpen,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

		ImGui::End();
	}
}

#endif