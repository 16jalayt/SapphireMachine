#pragma once
#include <fstream>

#include "Scene.h"
#include <Engine/GUI.h>
#include <Engine/SDL_ptr.h>
#include <vector>

class GUI : public Engine::GUI
{
public:
	GUI();
	void Draw();
private:
	void drawDebugMenu();

	bool debugMenuOpen = true;
};

using GUI_ptr = std::shared_ptr<GUI>;
