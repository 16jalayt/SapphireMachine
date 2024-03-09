#include "Loader.h"
#include <Engine/Graphics.h>
#include <loguru.hpp>
#include "Scene.h"

GUI_ptr currentGUI;

bool Loader::Boot()
{
	//Load preload scene
	loadScene("Boot");

	//
	//Archive::Load_Archive((char*)"data.dat");

	//Startup Flags:
	//set 81 to true
	flags[1081 - 1000] = true;

	UIInit();

	//Load initial scene
	loadScene("4604");

	return true;
}

void Loader::UIInit()
{
	//currentGUI = GUI_ptr(new GUIEngine());
	currentGUI = std::make_shared<GUI>();

	SDL_Rect CanvasRect = { 52, 18, 536, 292 };
	currentGUI->canvasRect = { (int)(CanvasRect.x * GlobalScale), (int)(CanvasRect.y * GlobalScale), (int)(CanvasRect.w * GlobalScale), (int)(CanvasRect.h * GlobalScale) };
	currentGUI->canvas = SDL_Texture_ptr(SDL_CreateTexture(Engine::Graphics::renderer.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, currentGUI->canvasRect.w, currentGUI->canvasRect.h));

	Sprite_ptr frame = std::make_shared<Engine::Sprite>("frame.png", 0, 0, RenderParent::window);
	currentGUI->AddSprite(frame);
}

void Loader::loadScene(int sceneName)
{
	Loader::loadScene(std::to_string(sceneName));
}

void Loader::loadScene(std::string sceneName)
{
	if (currentScene && sceneName == currentScene->sceneFile)
		ReloadScene();
	else
		_ChangeScene(sceneName);
}

//Scene files
std::ifstream Loader::getDataFile(std::string sceneName)
{
	if (sceneName.empty())
		return std::ifstream();

	std::ifstream inFile = std::ifstream("scenes/" + sceneName + ".hiff", std::ios::in | std::ios::binary | std::ios::ate);
	if (!inFile.fail()) {
		return inFile;
	}

	LOG_F(ERROR, "Could not open Scene file: %s", sceneName.c_str());
	return std::ifstream();
}

std::string Loader::getOVL(std::string ovlName)
{
	if (ovlName.empty())
		return std::string();

	std::ifstream inFile = std::ifstream("sprites/" + ovlName + ".png", std::ios::in | std::ios::binary | std::ios::ate);
	if (!inFile.fail()) {
		inFile.close();
		return "sprites/" + ovlName + ".png";
	}

	LOG_F(ERROR, "Could not open Sprite file: %s", ovlName.c_str());
	return std::string();
}

std::string Loader::getVideoPath(std::string backName)
{
	if (backName.empty())
		return std::string();

	std::ifstream inFile = std::ifstream("anims/" + backName + ".png", std::ios::in | std::ios::binary | std::ios::ate);
	if (!inFile.fail()) {
		inFile.close();
		return "anims/" + backName + ".png";
	}

	LOG_F(ERROR, "Cannot find background: %s", backName.c_str());
	return std::string();
}

std::string Loader::getSoundPath(std::string soundName)
{
	if (soundName.empty())
		return std::string();

	std::ifstream inFile("sound/" + soundName + ".ogg", std::ios::in | std::ios::binary | std::ios::ate);
	if (!inFile.fail()) {
		inFile.close();
		return "sound/" + soundName + ".ogg";
	}

	LOG_F(ERROR, "Cannot find sound: %s", soundName.c_str());
	return std::string();
}