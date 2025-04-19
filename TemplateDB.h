#pragma once
#ifndef TEMPLATEDB_H
#define TEMPLATEDB_H
#pragma warning(disable : 4996)
//#include "Actor_C.h"
#include <unordered_map>
#include <map>
#include <filesystem>
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <string>
#include "LuaHandler.h"
#include "gameEngineUtils.h"
#include "map"
#include "ParticleSystem498.h"
#include "Rigidbody.h"
class Actor_C;

class TemplateDB
{
	std::unordered_map<std::string, std::vector<std::pair<std::string,luabridge::LuaRef*>>> myTemplatesComponents;
	std::unordered_map<std::string, std::string> myTemplatesNames;
	LuaHandler* myLuaHandler;
public:
	
	TemplateDB() 
	{
	}

	void LoadTemplateDB(LuaHandler* handler) 
	{
		myLuaHandler = handler;
		
	}

	void ApplyTemplate(std::string templateName, std::map<std::string, luabridge::LuaRef*> & actorComponents, std::string & name, Actor_C* ptr) //Actor_C& actor, std::string templateName) 
	{
		// Check if template name exists and read it in
		if(!myTemplatesComponents.count(templateName) || !myTemplatesNames.count(templateName))
		{
			checkIfTemplateExists(templateName);
			std::string tmp = "resources/actor_templates/" + templateName + ".template";
			rapidjson::Document template_F;
			gameEngineUtils::ReadJsonFile(tmp, template_F);
			
			// get names
			if (template_F.HasMember("name"))
			{
				myTemplatesNames[templateName] = template_F["name"].GetString();
			}
			else 
			{
				myTemplatesNames[templateName] = "";
			}
			
			// create base code
			std::vector<std::pair<std::string, luabridge::LuaRef*>>& componentsToAdd = myTemplatesComponents[templateName];

			if (template_F.HasMember("components"))
			{
				
				int i = 0;
				for (auto current_component = template_F["components"].MemberBegin(); current_component != template_F["components"].MemberEnd(); current_component++)
				{
					// Add the component -- Duplicate keys will not be added
					std::string key = current_component->name.GetString();
					std::string type = current_component->value["type"].GetString();
					componentsToAdd.push_back(std::pair<std::string, luabridge::LuaRef*>(key, NULL));
					myLuaHandler->LoadComponent(componentsToAdd[i].second, key, type);

					luabridge::LuaRef comp = *componentsToAdd[i].second;
					
					// Inject values

					for (auto comp_property = current_component->value.MemberBegin(); comp_property != current_component->value.MemberEnd(); comp_property++)
					{
						if (comp_property->value.IsString())
						{
							comp[comp_property->name.GetString()] = comp_property->value.GetString();
						}
						else if (comp_property->value.IsFloat())
						{
							comp[comp_property->name.GetString()] = comp_property->value.GetFloat();
						}
						else if (comp_property->value.IsBool())
						{
							comp[comp_property->name.GetString()] = comp_property->value.GetBool();
						}
						else if (comp_property->value.IsInt())
						{
							comp[comp_property->name.GetString()] = comp_property->value.GetInt();
						}
						else
						{
							std::cout << "Undefined Property Type -- Template" << std::endl;
							exit(0);
						}

					}
					i++;
				}

			}

		}
		
		// update the values for the actors actual components
		name = myTemplatesNames[templateName];
		std::vector<std::pair<std::string, luabridge::LuaRef*>>& componentsToAdd = myTemplatesComponents[templateName];
		for (int i = 0; i < componentsToAdd.size(); i++) 
		{
			myLuaHandler->LoadTemplateComponent(actorComponents, componentsToAdd, i);

		}
		for (auto comp = actorComponents.begin(); comp != actorComponents.end() ; comp++ ) 
		{
			
			(*comp->second)["enabled"] = true;
			(*comp->second)["actor"] = ptr;
			
		}

	}

	static void checkIfTemplateExists(std::string TemplateName)
	{
		std::string tmp = "resources/actor_templates/" + TemplateName + ".template";
		if (!std::filesystem::exists(tmp))
		{
			gameEngineUtils::HandlePrint_PV("error: template " + TemplateName + " is missing", true);
			exit(0);
		}
		/*
		*/
	}

};

#endif

