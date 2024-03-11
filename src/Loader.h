#pragma once

#include "GUI.h"

class Loader
{
public:
	static bool Boot();
	static void UIInit();
	static void loadScene(int sceneName);
	static void loadScene(std::string sceneName);
	static std::ifstream getDataFile(std::string sceneName);
	static std::string getOVL(std::string ovlName);
	static std::string getVideoPath(std::string backName);
	static std::string getSoundPath(std::string soundName);
};

extern GUI_ptr currentGUI;