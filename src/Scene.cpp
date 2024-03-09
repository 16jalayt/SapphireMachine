#include "Scene.h"

#include "Loader.h"
#include <Engine/Config.h>
#include <Engine/GUI.h>
#include <Engine/Graphics.h>
#include <Engine/AudioClip.h>
#include <loguru.hpp>
#include "Audio.h"

Scene_ptr currentScene;
Scene_ptr nextScene;
bool sceneChangeFlag = false;
bool sceneReloadFlag = false;
std::string sceneChangeName;
std::string prevScene;
bool flags[576] = {};

Scene::Scene()
{
	sceneFile = "";
	sceneName = "";
}

void Scene::SetHeader(std::string description, std::string sceneFile)
{
	LOG_F(INFO, "\n\n\n\n*******New scene: %s: %s*******", sceneFile.c_str(), description.c_str());
	sceneName = sceneFile;
}

Scene::~Scene()
{
}

void Scene::Draw()
{
	if (this != NULL)
	{
		if (bkFMV)
		{
			SDL_SetRenderTarget(Engine::Graphics::renderer.get(), GUI::canvas.get());
			SDL_RenderCopy(Engine::Graphics::renderer.get(), bkFMV.get(), NULL, NULL);
			SDL_SetRenderTarget(Engine::Graphics::renderer.get(), NULL);
		}
		else if (bk)
			bk->Draw();

		for (auto& fmv : fmvs) {
			fmv->Draw();
		}

		for (auto& ovl : ovls) {
			ovl->Draw();
		}

		for (auto& hot : hots) {
			hot->Draw();
		}

		Audio::CheckTransitions();
	}
}

void Scene::EventProc(SDL_Event event)
{
	if (this != NULL && hots.size() != 0)
	{
		for (auto& hot : hots) {
			hot->Event(event);
		}
	}

	if (this != NULL && ovls.size() != 0)
	{
		for (auto& ovl : ovls) {
			ovl->Event(event);
		}
	}
}

void Scene::setBkg(std::string backName)
{
	if (backName.empty())
		return;

	LOG_F(INFO, "Background: %s", backName.c_str());

	std::string fileName = Loader::getVideoPath(backName);

	//don't need much validation. Sub objects will handle that.
	if (fileName.empty())
		return;

	//determine if video or graphic and open
	std::string ext = fileName.substr(fileName.find("."));

	if (ext == ".png" || ext == ".jpg")
	{
		bk = Sprite_ptr(new Engine::Sprite(fileName.c_str(), 0, 0));
	}
	else
	{
		LOG_F(ERROR, "Unknown background type in: %s", fileName.c_str());
	}
}

void Scene::AddSprite(Sprite_ptr sprite)
{
	ovls.push_back(sprite);
}

void Scene::AddHotzone(Button_ptr hot)
{
	hots.push_back(hot);
}

void Scene::AddMovie(Movie_ptr fmv)
{
	if (fmv)
		fmvs.push_back(fmv);
}

//create music paused
void Scene::AddMusic(std::string sound, int channel, int loop, int volL, int volR)
{
	Audio::AddMusic(sound, channel, loop, volL, volR);
}

//create audio paused
void Scene::AddSound(std::string sound, int channel, int loop, int volL, int volR)
{
	Audio::AddSound(sound, channel, loop, volL, volR);
}

//create audio paused with a scene to change to after ending
void Scene::AddSound(std::string sound, int channel, int loop, int volL, int volR, int scene)
{
	Audio::AddSound(sound, channel, loop, volL, volR, std::to_string(scene));
}

//create audio paused with a scene to change to after ending
void Scene::AddSound(std::string sound, int channel, int loop, int volL, int volR, std::string scene)
{
	Audio::AddSound(sound, channel, loop, volL, volR, scene);
}

//For internal use. Call Loader::loadScene instead
void _ChangeScene(std::string sceneName)
{
	if (sceneName != "9999")
	{
		sceneChangeName = sceneName;
		sceneChangeFlag = true;
	}
}

//For internal use. Call Loader::loadScene instead
void _LoadScene(std::string sceneName)
{
	nextScene = Scene_ptr(new Scene());

	//Reset current scene flags
	flags[0] = false;
	flags[1] = false;
	flags[2] = false;
	flags[3] = false;
	flags[4] = false;

	//TODO: call scene loader
	//SceneLoader::Load_Scene(sceneName);
	currentScene = nextScene;
	currentScene->Run();
}

void ReloadScene()
{
	Audio::RemoveAllSounds();
	currentScene->ovls.clear();
	currentScene->hots.clear();
	currentScene->fmvs.clear();
	//TODO: call scene loader
	//SceneLoader::Load_Scene(sceneName);
	currentScene->Run();
}

void Scene::Run()
{
	//Start playing movies
	/*for (auto& fmv : fmvs) {
		if (fmv->FMV)
			fmv->FMV->_paused = false;
	}*/

	if (!Engine::Config::debugNoSound)
	{
		Audio::currentMusic->Play();

		for (auto sound : Audio::sounds)
		{
			sound->Play();
		}
	}
}