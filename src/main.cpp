#define SDL_MAIN_HANDLED
//disable for insecure c functions and a warning about cxx17 standard
#pragma warning( disable : 4996 4038 )

#ifdef __SWITCH__
#include <switch.h>
#endif

#include <Engine/Config.h>
#include <loguru.hpp>

#ifdef __APPLE__
#include <unistd.h>
#endif

#include <Engine/Utils.h>
#include <Engine/Graphics.h>

#include "Scene.h"
#include "Loader.h"
#include "Utils.h"

#include "Cursor.h"
#include <Engine/SapphireApp.h>
#include "Engine/IMGUIInclude.h"

int main(int argc, char* argv[])
{
	SapphireApp_ptr app = std::make_unique<SapphireApp>(argc, argv);
	Utils::initLog();

	Loader::Boot();

	// loop variables
	int exit_requested = 0;
	SDL_Event event;

#if !defined(NO_IMGUI)
	//IMGUI does not like being in a dll
	ImGui::SetCurrentContext(currentGUI->imCtx);
	ImGuiIO& io = ImGui::GetIO();
#endif

#ifdef __SWITCH__
	while (!exit_requested && appletMainLoop())
#else
	while (!exit_requested)
#endif
	{
#if !defined(NO_IMGUI)
		//IMGUI does not like being in a dll
		ImGui::SetCurrentContext(currentGUI->imCtx);
#endif

		if (sceneReloadFlag)
		{
			ReloadScene();
		}
		if (sceneChangeFlag)
		{
			sceneChangeFlag = false;
			_LoadScene(sceneChangeName);
		}

		const Uint8* key_state = SDL_GetKeyboardState(NULL);

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				exit_requested = 1;
				break;
			}

			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(Engine::Graphics::window.get()))
			{
				exit_requested = 1;
				break;
			}

			currentGUI->EventProc(event);

			switch (event.type)
			{
				// use joystick
#ifdef __SWITCH__
			case SDL_JOYBUTTONDOWN:
				switch (event.jbutton.button)
				{
					/*case KEY_LSTICK_UP:
						if (wait > 0)
							wait--;
						break;
					case KEY_LSTICK_DOWN:
						if (wait < 100)
							wait++;
						break;*/
				case KEY_PLUS:
					LOG_F(ERROR, "\nShutting down engine\n");
					exit_requested = 1;
					break;
				}
#endif
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					LOG_F(ERROR, "\nShutting down engine\n");
					exit_requested = 1;
					break;
				}
				else if (event.key.keysym.sym == SDLK_c &&
					key_state[SDL_SCANCODE_LCTRL] &&
					key_state[SDL_SCANCODE_LSHIFT] &&
					key_state[SDL_SCANCODE_TAB])
				{
					if (Engine::Config::debugMenuEnabled)
						currentGUI->cheatSheetOpen = !currentGUI->cheatSheetOpen;
				}
#if !defined(NO_IMGUI)
				if (io.WantCaptureKeyboard)
#endif
					break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_FINGERDOWN:
#if !defined(NO_IMGUI)
				if (!io.WantCaptureMouse)
#endif
					//LOG_F(ERROR, "FingerDown");
					currentScene->EventProc(event);
				//LOG_F(ERROR, "Touch at: %d,%d\n", event.tfinger.x, event.tfinger.y);
				break;
			case SDL_MOUSEMOTION:
			case SDL_FINGERMOTION:
#if !defined(NO_IMGUI)
				if (!io.WantCaptureMouse)
				{
#endif
					//LOG_F(ERROR, "Fingermotion");
					//TODO: explicitly set to system cursor for IMGUI?
					Cursor::CursorChanged = false;
					currentScene->EventProc(event);
#if !defined(NO_IMGUI)
				}
#endif
				break;
				//TODO: resize window. Mainly for switch
				/*switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						LOG_F(ERROR, "\nShutting down engine\n");
						exit_requested = 1;
						break;
				default:
					break;
				}*/
			}
		}

		app->startFrame();

		currentScene->Draw();
		currentGUI->Draw();

		//TODO: re enable later
		//Cursor::DrawCursor();
		app->endFrame();
	}

	quit();
}