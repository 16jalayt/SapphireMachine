#include "Audio.h"
#include "Audio.h"
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "Engine/Utils.h"
#include "Engine/Config.h"
#include "Loader.h"
#include <loguru.hpp>
#include <Engine/AudioClip.h>

void Audio::AddSound(std::string sound, int channel, int loop, int volL, int volR)
{
	AddSound(sound, channel, loop, volL, volR, "");
}

void Audio::AddSound(std::string sound, int channel, int loop, int volL, int volR, std::string scene)
{
	AudioClip_ptr player = std::make_shared<Engine::AudioClip>();

	//if hdAudio then mus, else cdAudio chunk
	std::string path = Loader::getSoundPath(sound);
	if (path.empty() && sound != "silence")
	{
		LOG_F(ERROR, "Could not find sound: %s", sound.c_str());
		//Still need to store sound, so can transition
		player->ClipName = sound;
	}
	else
		player->ClipName = path;

	//TODO: put file format implementation here
	player->Clip = SDL_Mix_Chunk_ptr(Mix_LoadWAV(path.c_str()));
	if (player->Clip == NULL)
	{
		LOG_F(ERROR, "Failed to load sound: %s , %s", path.c_str(), Mix_GetError());
		return;
	}

	player->channel = channel;
	player->volL = volL;
	player->volR = volR;
	player->loop = loop;
	//Means just play, don't transition after
	if (scene != "9999" || scene != "")
		player->changeTo = scene;

	sounds.push_back(std::move(player));
}

void Audio::AddMusic(std::string sound, int channel, int loop, int volL, int volR)
{
	if (currentMusic && sound == currentMusic->ClipName)
		return;

	AudioClip_ptr player = std::make_shared<Engine::AudioClip>();

	std::string path = Loader::getSoundPath(sound);

	LOG_F(INFO, "Opening sound: %s as music.", path.c_str());

	//TODO:
	player->Music = SDL_Mix_Music_ptr(Mix_LoadMUS(path.c_str()));

	if (player->Music.get() == NULL && !Engine::Config::debugNoSound)
	{
		LOG_F(ERROR, "Failed to load music: %s , %s", path.c_str(), Mix_GetError());
	}

	player->ClipName = sound;
	player->channel = channel;
	player->volL = volL;
	player->volR = volR;
	player->loop = loop;

	currentMusic = player;
}

void Audio::CheckTransitions()
{
	//TODO: Doesn't transfer when file not found
	for (int i = 0; i < sounds.size(); i++)
	{
		//TODO: investigate null clip names being created
		//same remote button twice loops
		//clicker can still get stuck sometimes

		if (!sounds.at(i) || sounds.at(i)->ClipName == "")
			LOG_F(WARNING, "clip name empty or null");
		else
		{
			int test = Mix_Playing(sounds.at(i)->channel);
			//if sound not playing and scene transition set
			if (!sounds.at(i)->changeTo.empty() && Mix_Playing(sounds.at(i)->channel) == 0)
			{
				Loader::loadScene(sounds.at(i)->changeTo);
			}
		}
	}
}

void Audio::AddTransition(std::string scene)
{
	LOG_F(WARNING, "Audio::AddTransition Stub");
}