#pragma once

#include <string>
#include <vector>

#include <SDL2/SDL_mixer.h>
#include <Engine/AudioClip.h>
#include <Engine/Audio.h>
#include "Globals.h"
#include <memory>

class Audio : public Engine::Audio
{
public:
	static void AddSound(std::string sound, int channel, int loop, int chan1, int chan2);
	//TODO: add persist bool
	static void AddMusic(std::string sound, int channel, int loop, int chan1, int chan2);
	static void AddSound(std::string sound, int channel, int loop, int chan1, int chan2, std::string scene);
	static void AddTransition(std::string scene);
	static void CheckTransitions();

private:
};

using Audio_ptr = std::unique_ptr<Audio>;