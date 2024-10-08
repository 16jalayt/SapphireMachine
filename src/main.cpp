#define SDL_MAIN_HANDLED
//disable for insecure c functions and a warning about cxx17 standard
#pragma warning( disable : 4996 4038 )

#include <Engine\Config.h>
#include <loguru.hpp>

#ifdef __APPLE__
#include <unistd.h>
#endif

#ifdef __SWITCH__
#include <switch.h>
#endif

#include "globals.h"
#include <Engine\Utils.h>
#include <Engine\Graphics.h>
#include "Scene.h"
#include "Loader.h"
#include "Utils.h"
#include "Cursor.h"
#include <Engine\SapphireApp.h>
#include <imgui.h>
#include <iostream>

int main(int argc, char** argv)
{
	Engine::Config::parse(argc, argv);
	Utils::initLog(argc, argv);

	SapphireApp_ptr app = std::make_unique<SapphireApp>();
	//SDL_RenderSetLogicalSize(Engine::Graphics::renderer.get(), 400, 435);

	Loader::Boot();

	/*Scaled_Rect destRect = { 0,0, 300, 300 };
	//Scaled_Rect srcRect = { 0, 0, readInt(inFile), readInt(inFile) };
	Sprite_ptr ovl = std::make_shared<Engine::Sprite>(Loader::getOVL("139.png").c_str(), destRect.x, destRect.y, RenderParent::canvas, NULL);
	nextScene->AddSprite(ovl);*/

	// loop variables
	int exit_requested = 0;
	SDL_Event event;

#if !defined(__SWITCH__) && !defined(__APPLE__)
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
#if !defined(__SWITCH__) && !defined(__APPLE__)
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
				case KEY_MINUS:
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
#if !defined(__SWITCH__) && !defined(__APPLE__)
				if (io.WantCaptureKeyboard)
#endif
					break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_FINGERDOWN:
#if !defined(__SWITCH__) && !defined(__APPLE__)
				if (!io.WantCaptureMouse)
#endif
					//LOG_F(ERROR, "FingerDown");
					currentScene->EventProc(event);
				//LOG_F(ERROR, "Touch at: %d,%d\n", event.tfinger.x, event.tfinger.y);
				break;
			case SDL_MOUSEMOTION:
			case SDL_FINGERMOTION:
#if !defined(__SWITCH__) && !defined(__APPLE__)
				if (!io.WantCaptureMouse)
				{
#endif
					//LOG_F(ERROR, "Fingermotion");
					//TODO: explicitly set to system cursor for IMGUI?
					Cursor::CursorChanged = false;
					currentScene->EventProc(event);
#if !defined(__SWITCH__) && !defined(__APPLE__)
				}
#endif
				break;

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