#include "SceneLoader.h"
#include <loguru.hpp>
#include <json/json.h>
#include "Scene.h"
#include "Loader.h"

void SceneLoader::loadSceneFromJSON(std::string scenePath)
{
	scenePath += ".json";
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
	ifs.open(scenePath);

	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	Json::String errs;
	if (!parseFromStream(builder, ifs, &root, &errs)) {
		LOG_F(ERROR, "Error parsing json file %s: %s", scenePath.c_str(), errs.c_str());
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
			if (objects[i].isMember("rotation"))
			{
				int rotation = objects[i]["rotation"].asInt();
			}
			Sprite_ptr ovl = std::make_shared<Engine::Sprite>(name.c_str(), x, y, RenderParent::canvas, texture_rect);
			nextScene->AddSprite(ovl);
		}
	}
	catch (Json::LogicError& e)
	{
		LOG_F(ERROR, "Error parsing scene json: %s", e.what());
		quit();
	}
}