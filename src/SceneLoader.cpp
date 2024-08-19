#include "SceneLoader.h"
#include <loguru.hpp>
#include <json/json.h>
#include "Scene.h"
#include "Loader.h"

void SceneLoader::loadSceneFromJSON(std::string scenePath)
{
	//scenePath += ".json";
	//TODO: No error on not found. Validate stream first
	/*std::ifstream ifs("test.json");
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(ifs, root))
	{
		LOG_F(ERROR, "Error parsing json: %s", reader.getFormattedErrorMessages().c_str());
		quit();
	}*/

	Json::Value root;
	std::ifstream ifs;
	//ifs.open(scenePath);
	std::string sceneFilePath = Loader::getDataFile(scenePath);
	if (sceneFilePath.empty())
	{
		LOG_F(ERROR, "Failed to load scene: %s", scenePath.c_str());
		quit();
	}
	ifs.open(sceneFilePath);

	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	Json::String errs;
	if (!parseFromStream(builder, ifs, &root, &errs)) {
		LOG_F(ERROR, "Error parsing json file %s: %s", sceneFilePath.c_str(), errs.c_str());
		//TODO: not straight quit
		quit();
	}

	try
	{
		nextScene->setBkg(root["sceneBack"].asString());

		if (root.isMember("colormod"))
		{
			Json::Value& mod = root["colormod"];
			if (mod.size() == 3)
			{
				nextScene->bk->setColorMod(mod[0].asInt(), mod[1].asInt(), mod[2].asInt());
			}
			else
				LOG_F(WARNING, "\nJSON colormod parameter incorect length");
		}

		const Json::Value& objects = root["objects"]; // array

		//for (auto const& id : characters.getMemberNames()) {
			//std::cout << id << std::endl;
		//}

		for (unsigned int i = 0; i < objects.size(); i++) {
			//Ignore name. Just to make json clearer
			//objects[i]["name"].asString();
			std::string name = Loader::getOVL(objects[i]["image"].asString());
			int x = objects[i]["x"].asInt();
			int y = objects[i]["y"].asInt();
			Scaled_Rect pos_rect = { x, y, -1, -1 };

			Scaled_Rect texture_rect = Scaled_Rect();
			if (objects[i].isMember("Texture Rect"))
			{
				const Json::Value mod = objects[i]["Texture Rect"];
				if (mod.size() == 4)
				{
					texture_rect = { mod[0].asInt(), mod[1].asInt(), mod[2].asInt(), mod[3].asInt() };
				}
				else
					LOG_F(WARNING, "\nJSON Texture Rect parameter incorect length");
			}

			//TODO: make way to set disabled later than constructor
			Button_ptr ovl = std::make_shared<Engine::Button>(pos_rect, name.c_str(), RenderParent::canvas, true, texture_rect);
			nextScene->AddHotzone(ovl);

			if (objects[i].isMember("rotation"))
			{
				ovl->rotation = objects[i]["rotation"].asInt();
			}
			if (objects[i].isMember("flip"))
			{
				std::string flipState = objects[i]["flip"].asString();
				if (flipState.find("H") != std::string::npos)
					ovl->flipped = (SDL_RendererFlip)(ovl->flipped | SDL_FLIP_HORIZONTAL);
				if (flipState.find("V") != std::string::npos)
					ovl->flipped = (SDL_RendererFlip)(ovl->flipped | SDL_FLIP_VERTICAL);
			}
			if (objects[i].isMember("displayIf"))
			{
				ovl->visible(flags[objects[i]["displayIf"].asInt()]);
				//TODO: either visibility property or surround the creation.
				//some way to update
			}
			if (objects[i].isMember("setOnClick"))
			{
				int changeTo = objects[i]["setOnClick"].asInt();
				//TODO: either visibility property or surround the creation.
				ovl->callback = [changeTo = changeTo]
					{
						//Toggle Value
						flags[changeTo] = !flags[changeTo];
					};
			}
			if (objects[i].isMember("loadScene"))
			{
				std::string changeTo = objects[i]["loadScene"].asString();
				ovl->callback = [changeTo = changeTo]
					{
						Loader::loadScene(changeTo);
					};
			}
			/*hotZone->hover_event = [cursorNumber = cursorNumber]
				{
					Cursor::setCursor(cursorNumber);
				};*/
		}
	}
	catch (Json::LogicError& e)
	{
		LOG_F(ERROR, "Error parsing scene json: %s", e.what());
		quit();
	}
}