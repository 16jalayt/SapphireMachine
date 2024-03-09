#include "Movie.h"
#include <loguru.hpp>

Movie::Movie(std::string fileName, int x, int y, bool isLooped, RenderParent parent, Scaled_Rect partial)
{
	//determine if video or graphic and open
	std::string ext = fileName.substr(fileName.find("."));

	//Load whatever type of movie or animeted sprite
	if (ext == ".anim")
	{
		//std::vector<SDL_Texture_ptr> test = ANIM::parseAnim(fileName.c_str());
		std::vector<SDL_Texture_ptr> test;
		if (test.empty())
		{
			LOG_F(WARNING, "Movie returned empty for: %s\n", fileName.c_str());
			return;
		}
		ASprite = std::make_unique<Engine::AnimatedSprite>(std::move(test), x, y);
		ASprite->looped = isLooped;
	}
	else
		LOG_F(WARNING, "Unknown Movie type in: %s\n", fileName.c_str());
}

void Movie::Draw()
{
	if (this != NULL)
	{
		if (ASprite)
			ASprite->Draw();
	}
}

Movie::~Movie()
{
	//if (FMV)
		//FMV->Close();
}