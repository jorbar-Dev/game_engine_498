#pragma once
#ifndef ACTOR_C_H
#define ACTOR_C_H
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "glm/glm.hpp"
#include "SDL2/SDL.h"
#include "Helper.h"
#include <optional>
#include "LuaHandler.h"
#include "TemplateDB.h"
#include <unordered_map>
#include "collision.h"
//class LuaHandler;

#include "document.h"

class Actor_C
{
	


	public:
		std::string actor_name = "";
		std::unordered_map<std::string, luabridge::LuaRef*> components_with_on_start_ran;
		std::vector<std::pair<std::string, luabridge::LuaRef*>> components_with_on_start;
		std::vector<std::pair<std::string, luabridge::LuaRef*>> components_with_on_update;
		std::vector<std::pair<std::string, luabridge::LuaRef*>> components_with_on_late_update;

		std::vector<std::pair<std::string, luabridge::LuaRef*>> components_with_on_collision_enter;
		std::vector<std::pair<std::string, luabridge::LuaRef*>> components_with_on_collision_exit;

		std::vector<std::pair<std::string, luabridge::LuaRef*>> components_with_on_trigger_enter;
		std::vector<std::pair<std::string, luabridge::LuaRef*>> components_with_on_trigger_exit;
		
		int OnUpdateIndex;
		int OnStartIndex;
		int OnLateUpdateIndex;
		bool Alive;
		bool DoNotDestroyOnLoad = false;

		int ID = -1;
		int Index = -1;	
		std::map<std::string, luabridge::LuaRef*> myComponents;
		LuaHandler* luaHandler;
		int* numComponentsAdded;
		std::map<int,Actor_C*>* need_lifecycle_update;
		

		void OnCollisionEnter(collision other) 
		{
			std::string update = "OnCollisionEnter";
			for (int i = 0; i < components_with_on_collision_enter.size(); i++)
			{
				// std::cout << "Calling On Start: " + (*onStart[i].second)["type"].tostring() << std::endl;
				luabridge::LuaRef func = (*components_with_on_collision_enter[i].second)[update];
				bool enabled = (*components_with_on_collision_enter[i].second)["enabled"].cast<bool>();

				try
				{
					if (enabled == true)
					{
						func((*components_with_on_collision_enter[i].second), other);
					}
				}
				catch (const luabridge::LuaException& e)
				{
					std::string error_message = e.what();
					std::replace(error_message.begin(), error_message.end(), '\\', '/');
					std::cout << "\033[31m" << actor_name << " : " << error_message << "\033[0m" << std::endl;
				}

			}
		}
		void OnCollisionExit(collision other)
		{
			std::string update = "OnCollisionExit";
			for (int i = 0; i < components_with_on_collision_exit.size(); i++)
			{
				// std::cout << "Calling On Start: " + (*onStart[i].second)["type"].tostring() << std::endl;
				luabridge::LuaRef func = (*components_with_on_collision_exit[i].second)[update];
				bool enabled = (*components_with_on_collision_exit[i].second)["enabled"].cast<bool>();

				try
				{
					if (enabled == true)
					{
						func((*components_with_on_collision_exit[i].second), other);
					}
				}
				catch (const luabridge::LuaException& e)
				{
					std::string error_message = e.what();
					std::replace(error_message.begin(), error_message.end(), '\\', '/');
					std::cout << "\033[31m" << actor_name << " : " << error_message << "\033[0m" << std::endl;
				}

			}

		}

		void OnTriggerEnter(collision other)
		{
			std::string update = "OnTriggerEnter";
			for (int i = 0; i < components_with_on_trigger_enter.size(); i++)
			{
				// std::cout << "Calling On Start: " + (*onStart[i].second)["type"].tostring() << std::endl;
				luabridge::LuaRef func = (*components_with_on_trigger_enter[i].second)[update];
				bool enabled = (*components_with_on_trigger_enter[i].second)["enabled"].cast<bool>();

				try
				{
					if (enabled == true)
					{
						func((*components_with_on_trigger_enter[i].second), other);
					}
				}
				catch (const luabridge::LuaException& e)
				{
					std::string error_message = e.what();
					std::replace(error_message.begin(), error_message.end(), '\\', '/');
					std::cout << "\033[31m" << actor_name << " : " << error_message << "\033[0m" << std::endl;
				}

			}
		}
		void OnTriggerExit(collision other)
		{
			std::string update = "OnTriggerExit";
			for (int i = 0; i < components_with_on_trigger_exit.size(); i++)
			{
				// std::cout << "Calling On Start: " + (*onStart[i].second)["type"].tostring() << std::endl;
				luabridge::LuaRef func = (*components_with_on_trigger_exit[i].second)[update];
				bool enabled = (*components_with_on_trigger_exit[i].second)["enabled"].cast<bool>();

				try
				{
					if (enabled == true)
					{
						func((*components_with_on_trigger_exit[i].second), other);
					}
				}
				catch (const luabridge::LuaException& e)
				{
					std::string error_message = e.what();
					std::replace(error_message.begin(), error_message.end(), '\\', '/');
					std::cout << "\033[31m" << actor_name << " : " << error_message << "\033[0m" << std::endl;
				}

			}

		}

		int GetID()
		{
			return ID;
		}
		std::string GetName()
		{
			return actor_name;
		}
		luabridge::LuaRef GetComponentByKey(std::string key) 
		{
			if (myComponents.count(key) > 0 && (*myComponents[key])["enabled"].cast<bool>())
			{
				return *myComponents[key];
			}
			return luabridge::LuaRef(luaHandler->GetLuaState());
		}
		luabridge::LuaRef GetComponent(std::string key)
		{
			luabridge::LuaRef* best = NULL;
			if (myComponents.size() > 0)
			{
				for (auto i = myComponents.begin(); i != myComponents.end(); i++) 
				{



					if ((*i->second)["type"].tostring() == key  && ((*i->second)["enabled"].cast<bool>() == true)  && (best == NULL ))
					{
						best = i->second;
					}
					else if ((*i->second)["type"].tostring() == key && ((*i->second)["enabled"].cast<bool>() == true) && best != NULL &&(i->first < (*best)["key"].tostring()))
					{
						best = i->second;
					}
				}

				if (best != NULL) 
				{
					if (best->isNil()) 
					{
					
						std::cout << " hello am I true" << std::endl;
					}
					return *best;
				}

			}
			if (key == "Donna" )
			{
				return luabridge::LuaRef(luaHandler->GetLuaState());
			}
			return luabridge::LuaRef(luaHandler->GetLuaState());
		}
		luabridge::LuaRef GetComponents(std::string key)
		{
			std::vector<std::pair<std::string, luabridge::LuaRef*>> validComponents;
			
			for (auto i = myComponents.begin(); i != myComponents.end(); i++)
			{
				if ((*i->second)["type"].tostring() == key && (*i->second)["enabled"].cast<bool>())
				{
					validComponents.emplace_back(std::pair<std::string, luabridge::LuaRef*>(i->first, i->second));
				}
			}
			
			std::sort(validComponents.begin(), validComponents.end(), [](const std::pair<std::string, luabridge::LuaRef*>& first, const std::pair<std::string, luabridge::LuaRef*>& second)
				{
					return first.first < second.first;
				});
			
			
			// create a new lua table
			// iterate through each element and push a poi
			int j = 1;
			luabridge::LuaRef table = luabridge::LuaRef(luabridge::newTable(luaHandler->GetLuaState()));
			for (auto i = validComponents.begin(); i != validComponents.end(); i++)
			{
				table[j] = *i->second;
				j++;
			}
			return table;
		}

		~Actor_C()
		{

		}

		Actor_C(LuaHandler* myLuaHandler, TemplateDB* myTemplateDB, const rapidjson::Value* parameters, int id, int index, int* numComponentsAddedDuringGame, std::map<int, Actor_C*>* updateCheck)
		{
			Alive = true;
			need_lifecycle_update = updateCheck;
			numComponentsAdded = numComponentsAddedDuringGame;
			luaHandler = myLuaHandler;
			if (parameters->HasMember("template"))
			{
				myTemplateDB->ApplyTemplate((*parameters)["template"].GetString(), myComponents, actor_name,this);

				if (!myComponents.empty())
				{
					
					for (auto comp = myComponents.begin(); comp != myComponents.end(); comp++)
					{
						(*(comp)->second)["actor"] = *this;

						(*(comp)->second)["enabled"] = true;
					}
				}

			}
			if (parameters->HasMember("name"))
			{
				actor_name = (*parameters)["name"].GetString();
			}
			if (parameters->HasMember("components"))
			{
				
				for (auto current_component = (*parameters)["components"].MemberBegin(); current_component != (*parameters)["components"].MemberEnd(); current_component++)
				{
					// Add the component -- Duplicate keys will not be added
					std::string key = current_component->name.GetString();
					if (current_component->value.HasMember("type")) 
					{
						std::string type = current_component->value["type"].GetString();
						AddComponent_CPP(type, key);
						
					}

					luabridge::LuaRef comp = *myComponents[key];

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
							std::cout << "Undefined Property Type" << std::endl;
							exit(0);
						}
					}
				}
			}
			UpdateLifeCycleVectors(true);	
			ID = id;
			Index = index;
			
		}

		Actor_C(LuaHandler* myLuaHandler, TemplateDB* myTemplateDB, std::string template_name, int id, int index, int* numComponentsAddedDuringGame, std::map<int, Actor_C*>* updateCheck)
		{
			Alive = true;
			need_lifecycle_update = updateCheck;
			numComponentsAdded = numComponentsAddedDuringGame;
			luaHandler = myLuaHandler;
			myTemplateDB->ApplyTemplate(template_name, myComponents, actor_name,this);
			

			ID = id;
			Index = index;
			(*need_lifecycle_update)[this->ID] = this;

		}

		inline void AddComponent_CPP(std::string type, std::string key)
		{
			luaHandler->LoadComponent(myComponents, key, type);
			
			(*myComponents[key])["actor"] = this;
			(*myComponents[key])["enabled"] = true;
			(*myComponents[key])["type"] = type;
			(*myComponents[key])["key"] = key;

		}

		inline luabridge::LuaRef AddComponent(std::string type)
		{
			std::string key = "r" + std::to_string( *numComponentsAdded);
			*numComponentsAdded += 1;
			AddComponent_CPP(type, key);
			(*need_lifecycle_update)[this->ID]= this;
			return *myComponents[key];
		}

		inline void RemoveComponent(luabridge::LuaRef component_ref)
		{
			(component_ref)["enabled"] = false;
			luabridge::LuaRef func = (component_ref)["OnDestroy"];
			try
			{
				if (component_ref["OnDestroy"].isFunction())
				{
					func(component_ref);
				}
			}
			catch (const luabridge::LuaException& e)
			{
				std::string error_message = e.what();
				std::replace(error_message.begin(), error_message.end(), '\\', '/');
				std::cout << "\033[31m" << actor_name << " : " << error_message << "\033[0m" << std::endl;
			}


		}

		inline void Destroy() 
		{
			Alive = false;
			for (auto comp = myComponents.begin(); comp != myComponents.end(); comp++)
			{
				RemoveComponent(*comp->second);
			}
		}

		inline void UpdateLifeCycleVectors(bool enqueue) 
		{
			//components_with_on_start_ran.clear();
			components_with_on_start.clear();
			components_with_on_update.clear();
			components_with_on_late_update.clear();
			components_with_on_trigger_enter.clear();
			components_with_on_trigger_exit.clear();
			if (!myComponents.empty())
			{
				std::string OnStart = "OnStart";
				std::string OnUpdate = "OnUpdate";
				std::string OnLateUpdate = "OnLateUpdate";
				std::string OnCollisionEnter = "OnCollisionEnter";
				std::string OnCollisionExit = "OnCollisionExit";
				std::string OnTriggerEnter = "OnTriggerEnter";
				std::string OnTriggerExit = "OnTriggerExit";
				for (auto comp = myComponents.begin(); comp != myComponents.end(); comp++)
				{
					
					if ((*comp->second)[OnStart].isFunction() && components_with_on_start_ran.count(comp->first) == 0)
					{
						components_with_on_start_ran[comp->first] = comp->second;
						components_with_on_start.push_back(std::pair(comp->first, myComponents[comp->first]));
					}
					if ((*comp->second)[OnUpdate].isFunction())
					{
						components_with_on_update.push_back(std::pair(comp->first, myComponents[comp->first]));
					}
					if ((*comp->second)[OnLateUpdate].isFunction())
					{
						components_with_on_late_update.push_back(std::pair(comp->first, myComponents[comp->first]));
					}
					if ((*comp->second)[OnCollisionEnter].isFunction())
					{
						components_with_on_collision_enter.push_back(std::pair(comp->first, myComponents[comp->first]));
					}
					if ((*comp->second)[OnCollisionExit].isFunction())
					{
						components_with_on_collision_exit.push_back(std::pair(comp->first, myComponents[comp->first]));
					}

					if ((*comp->second)[OnTriggerEnter].isFunction())
					{
						components_with_on_trigger_enter.push_back(std::pair(comp->first, myComponents[comp->first]));
					}
					if ((*comp->second)[OnTriggerExit].isFunction())
					{
						components_with_on_trigger_exit.push_back(std::pair(comp->first, myComponents[comp->first]));
					}


				}

				std::sort(components_with_on_start.begin(), components_with_on_start.end(), [](const std::pair<std::string, luabridge::LuaRef*>& first, const std::pair<std::string, luabridge::LuaRef*>& second)
					{
						return first.first < second.first;
					});
				std::sort(components_with_on_update.begin(), components_with_on_update.end(), [](const std::pair<std::string, luabridge::LuaRef*>& first, const std::pair<std::string, luabridge::LuaRef*>& second)
					{
						return first.first < second.first;
					});
				std::sort(components_with_on_late_update.begin(), components_with_on_late_update.end(), [](const std::pair<std::string, luabridge::LuaRef*>& first, const std::pair<std::string, luabridge::LuaRef*>& second)
					{
						return first.first < second.first;
					});
				std::sort(components_with_on_trigger_enter.begin(), components_with_on_trigger_enter.end(), [](const std::pair<std::string, luabridge::LuaRef*>& first, const std::pair<std::string, luabridge::LuaRef*>& second)
					{
						return first.first < second.first;
					});
				std::sort(components_with_on_trigger_exit.begin(), components_with_on_trigger_exit.end(), [](const std::pair<std::string, luabridge::LuaRef*>& first, const std::pair<std::string, luabridge::LuaRef*>& second)
					{
						return first.first < second.first;
					});

				std::sort(components_with_on_collision_enter.begin(), components_with_on_collision_enter.end(), [](const std::pair<std::string, luabridge::LuaRef*>& first, const std::pair<std::string, luabridge::LuaRef*>& second)
					{
						return first.first < second.first;
					});
				std::sort(components_with_on_collision_exit.begin(), components_with_on_collision_exit.end(), [](const std::pair<std::string, luabridge::LuaRef*>& first, const std::pair<std::string, luabridge::LuaRef*>& second)
					{
						return first.first < second.first;
					});

				if (enqueue) 
				{
					OnStartIndex = luaHandler->EnQueueOnStartCalls(components_with_on_start, actor_name);
					OnUpdateIndex = luaHandler->EnQueueOnUpdateCalls(components_with_on_update, actor_name);
					OnLateUpdateIndex = luaHandler->EnQueueOnLateUpdateCalls(components_with_on_late_update, actor_name);
				}
				else 
				{
					OnStartIndex = luaHandler->EnQueueOnStartCalls(components_with_on_start, actor_name);
					luaHandler->ReQueueOnUpdateCalls(&components_with_on_update, OnUpdateIndex);
					luaHandler->ReQueueOnLateUpdateCalls(&components_with_on_late_update, OnLateUpdateIndex);
				}

			}
		
		}

};



#endif

