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
#include <Engine/GUI.h>
#include "Cursor.h"
#include <Engine/Config.h>

GUI::GUI() :Engine::GUI()
{
	Cursor::Init();
}

void GUI::Draw()
{
	Engine::GUI::Draw();
	//TODO: re enable
	//drawDebugMenu();
}

#if !defined(__SWITCH__) && !defined(__APPLE__)
void GUI::drawDebugMenu()
{
	if (Engine::Config::debugMenuEnabled)
	{
		ImGui::SetNextWindowSize(ImVec2(700, 500));

		//To make minimized at start
		//ImGui::SetNextWindowCollapsed(true);

		// Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Begin("Debug Menu", &Engine::Config::debugMenuEnabled,
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

		ImGui::End();
	}
}

#endif