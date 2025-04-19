#pragma once
#ifndef LUA_HANDLER_H
#define LUA_HANDLER_H

#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include "LuaSupportFunctions.h"
#include "InputManager.h"
#include "RendererHandler.h"
#include "map"
#include "box2d/box2d.h"
#include "BuildInComponents.h"
#include "Rigidbody.h"
#include "ParticleSystem498.h"
#include "SpriteRenderer_C.h"



class LuaHandler
{
	lua_State* lua_state;

	std::vector<std::vector<std::pair<std::string, luabridge::LuaRef*>>*> onStart;
	std::vector < std::vector<std::pair<std::string, luabridge::LuaRef*>>*> onUpdate;
	std::vector < std::vector<std::pair<std::string, luabridge::LuaRef*>>*> onLateUpdate;
	std::vector<std::string> onStartNames;
	std::vector<std::string> onUpdateNames;
	std::vector<std::string> onLateUpdateNames;
	std::unordered_map<std::string, std::string> valid_components;
	std::vector<int> onStartDequeue;
	std::vector<int> onUpdateDequeue;
	std::vector<int> onLateUpdateDequeue;

	Rigidbody parent_RB;
	ParticleSystem498 parent_PS;
	SpriteRenderer_C parent_SPC;

public:

	void LoadLuaHandler() 
	{
		lua_state = luaL_newstate();
		luaL_openlibs(lua_state);
		LoadGameEngineAPIs();
		LoadLuaFiles();
		TTF_Init();
		luabridge::getGlobalNamespace(lua_state)
			.beginNamespace("Text")
			.addFunction("Draw", &RendererHandler::MakeDrawTextRequest)
			.endNamespace();
	}

	inline lua_State* GetLuaState() 
	{
		return lua_state;
	}


	// Function Definitions [TODO]: move to seperate file for readability




	// setting up actor with components
		// Establish Inheritance

	inline void LoadComponent(std::map<std::string, luabridge::LuaRef*>& myComponents, const std::string& key, const std::string& new_component)
	{
		if (myComponents.count(key) > 0)
		{
			return;
		}
		if (!(valid_components.count(new_component) > 0))
		{
			std::cout << "error: failed to locate component " + new_component;
			exit(0);
		}

		if (new_component == "Rigidbody") 
		{
			Rigidbody* new_component = new Rigidbody(parent_RB);
			myComponents[key] = new luabridge::LuaRef(lua_state, new_component);
			
		}
		else if ("ParticleSystem" == new_component)
		{
			ParticleSystem498* new_component = new ParticleSystem498(parent_PS);
			myComponents[key] = new luabridge::LuaRef(lua_state, new_component);
		}
		else if ("SpriteRenderer_C" == new_component)
		{
			SpriteRenderer_C* new_component = new SpriteRenderer_C(parent_SPC);
			myComponents[key] = new luabridge::LuaRef(lua_state, new_component);
		}
		else 
		{
			luabridge::LuaRef original = luabridge::getGlobal(lua_state, new_component.c_str());
			myComponents[key] = new luabridge::LuaRef(luabridge::newTable(lua_state));
			EstablishInheritance(*myComponents[key], original);
			(*myComponents[key])["key"] = key;
		}
		//std::cout << "Component Loaded" << std::endl;
	}

	inline void LoadComponent(luabridge::LuaRef* & component, const std::string& key,const std::string& new_component)
	{
		if (!(valid_components.count(new_component) > 0))
		{
			std::cout << "error: failed to locate component " + new_component;
			exit(0);
		}
		if (new_component == "Rigidbody") 
		{
			Rigidbody* new_component = new Rigidbody(parent_RB);
			component = new luabridge::LuaRef(lua_state, new_component);
			return;
		}
		if (new_component == "ParticleSystem") 
		{
			ParticleSystem498* new_component = new ParticleSystem498(parent_PS);
			component = new luabridge::LuaRef(lua_state, new_component);
			return;
		}
		if (new_component == "SpriteRenderer_C")
		{
			SpriteRenderer_C* new_component = new SpriteRenderer_C(parent_SPC);
			component = new luabridge::LuaRef(lua_state, new_component);
			return;
		}


		luabridge::LuaRef original = luabridge::getGlobal(lua_state, new_component.c_str());
		component = new luabridge::LuaRef(luabridge::newTable(lua_state));
		EstablishInheritance(*component, original);
		(*component)["key"] = key;
		//std::cout << "Component Loaded" << std::endl;
	}

	inline void LoadTemplateComponent(std::map<std::string, luabridge::LuaRef*>& actorComponents, std::vector<std::pair<std::string, luabridge::LuaRef*>>& componentsToAdd, int i)
	{
		std::string tmp = "type";
		std::pair<std::string, luabridge::LuaRef*> pair = componentsToAdd[i];
		luabridge::LuaRef* ptr = pair.second;
		if ("Rigidbody" == (*ptr)["type"].tostring()) 
		{
			// win
			Rigidbody* new_component = new Rigidbody((*(componentsToAdd[i].second)->cast<Rigidbody*>()));
			actorComponents[componentsToAdd[i].first] = new luabridge::LuaRef(lua_state, new_component);
		}
		else if ("ParticleSystem" == (*ptr)["type"].tostring())
		{
			ParticleSystem498* new_component = new ParticleSystem498((*(componentsToAdd[i].second)->cast<ParticleSystem498*>()));
			actorComponents[componentsToAdd[i].first] = new luabridge::LuaRef(lua_state, new_component);
		}
		else if ("SpriteRenderer_C" == (*ptr)["type"].tostring())
		{
			SpriteRenderer_C* new_component = new SpriteRenderer_C((*(componentsToAdd[i].second)->cast<SpriteRenderer_C*>()));
			actorComponents[componentsToAdd[i].first] = new luabridge::LuaRef(lua_state, new_component);
		}
		else 
		{
			(actorComponents[componentsToAdd[i].first]) = new luabridge::LuaRef(luabridge::newTable(lua_state));
			EstablishInheritance(*(actorComponents[componentsToAdd[i].first]), *(componentsToAdd[i].second));
		}
	}

	inline void EstablishInheritance(luabridge::LuaRef& instance_table, luabridge::LuaRef & parent_table) 
	{
		// course code
		// We must create a metatable to establish inheritance in lua
		luabridge::LuaRef new_metatable = luabridge::newTable(lua_state);
		new_metatable["__index"] = parent_table;

		// we must use the raw lua C-API (lua stack) to perform a setmetatbale operation.
		instance_table.push(lua_state);
		new_metatable.push(lua_state);
		lua_setmetatable(lua_state, -2);
		lua_pop(lua_state, 1);

	}

	inline void EstablishInheritanceCPP(luabridge::LuaRef& instance_table, luabridge::LuaRef& parent_table)
	{
		// TODO parse a lua ref and pass the correct values

	}

	// OnStart functionality

		// Queue an on start call
	inline int EnQueueOnStartCalls(std::vector<std::pair<std::string, luabridge::LuaRef*>>& toEnqueue, std::string actor_name) 
	{
		
		onStart.push_back(&toEnqueue);
		onStartNames.push_back(actor_name);
		return onStart.size() - 1;
	}

	inline int EnQueueOnUpdateCalls(std::vector<std::pair<std::string, luabridge::LuaRef*>>& toEnqueue, std::string actor_name)
	{
		onUpdate.push_back(&toEnqueue);
		onUpdateNames.push_back(actor_name);
		return onUpdate.size() - 1;
	}

	inline int EnQueueOnLateUpdateCalls(std::vector<std::pair<std::string, luabridge::LuaRef*>>& toEnqueue, std::string actor_name)
	{
		
		onLateUpdate.push_back(&toEnqueue);
		onLateUpdateNames.push_back(actor_name);
		return onLateUpdate.size() - 1;
	}

	inline void ReQueueOnStartCalls(std::vector<std::pair<std::string, luabridge::LuaRef*>>* toDequeue, int index)
	{
		onStart[index] = toDequeue;
	}

	inline void ReQueueOnUpdateCalls(std::vector<std::pair<std::string, luabridge::LuaRef*>>* toDequeue,int index)
	{
		
		onUpdate[index] = toDequeue;
	}

	inline void ReQueueOnLateUpdateCalls(std::vector<std::pair<std::string, luabridge::LuaRef*>>* toDequeue, int index)
	{
		
		onLateUpdate[index] = toDequeue;
	}

	void OnStart() 
	{
		for (int i = 0; i < onStart.size(); i++) 
		{
			IterateThroughUpdate("OnStart", *onStart[i], onStartNames[i]);
		}
		onStart.clear();
		onStartNames.clear();
	}

	void OnUpdate() 
	{
		for (int i = 0; i < onUpdate.size(); i++)
		{
			IterateThroughUpdate("OnUpdate", *onUpdate[i], onUpdateNames[i]);
		}
	}

	void OnLateUpdate()
	{
		for (int i = 0; i < onUpdate.size(); i++)
		{
			IterateThroughUpdate("OnLateUpdate", *onLateUpdate[i], onLateUpdateNames[i]);
		}

	}

	inline void IterateThroughUpdate(std::string update, std::vector<std::pair<std::string, luabridge::LuaRef*>>& list, std::string& name)
	{
		for (int i = 0; i < list.size(); i++)
		{
			// std::cout << "Calling On Start: " + (*onStart[i].second)["type"].tostring() << std::endl;
			luabridge::LuaRef func = (*list[i].second)[update];
			bool enabled = (*list[i].second)["enabled"].cast<bool>();

			try
			{
				if (enabled == true)
				{
					func((*list[i].second));
				}
			}
			catch (const luabridge::LuaException& e)
			{
				ReportError(name, e);
			}

		}
	}

	inline void IterateThroughUpdateCollision(std::string update, std::vector<std::pair<std::string, luabridge::LuaRef*>>& list, std::string& name, uintptr_t* other)
	{
		for (int i = 0; i < list.size(); i++)
		{
			// std::cout << "Calling On Start: " + (*onStart[i].second)["type"].tostring() << std::endl;
			luabridge::LuaRef func = (*list[i].second)[update];
			bool enabled = (*list[i].second)["enabled"].cast<bool>();

			try
			{
				if (enabled == true)
				{
					func((*list[i].second),other);
				}
			}
			catch (const luabridge::LuaException& e)
			{
				ReportError(name, e);
			}

		}
	}

	void ReportError(const std::string& actor_name, const luabridge::LuaException& e)
	{
		std::string error_message = e.what();
		std::replace(error_message.begin(), error_message.end(), '\\', '/');
		std::cout << "\033[31m" << actor_name << " : " << error_message << "\033[0m" << std::endl;

	}
private:


	// Private functions for lua Handler

	void LoadGameEngineAPIs() 
	{
		luabridge::getGlobalNamespace(lua_state)
			.beginNamespace("Debug")
			.addFunction("Test", &LuaSupportFunctions::TestFunc)
			.addFunction("Log",&LuaSupportFunctions::DebugLog)
			.addFunction("LogError", &LuaSupportFunctions::DebugLogError)
			.endNamespace();

		luabridge::getGlobalNamespace(lua_state)
			.beginNamespace("Application")
			.addFunction("Quit", &LuaSupportFunctions::QuitApplication)
			.addFunction("Sleep", &LuaSupportFunctions::ApplicationSleep)
			.addFunction("GetFrame", &LuaSupportFunctions::GetFrameNumber)
			.addFunction("OpenURL", &LuaSupportFunctions::OpenURL)
			.endNamespace();

		luabridge::getGlobalNamespace(lua_state)
			.beginClass<glm::vec2>("vec2")
			.addProperty("x", &glm::vec2::x)
			.addProperty("y", &glm::vec2::y)
			.endClass();
luabridge::getGlobalNamespace(lua_state)
			.beginNamespace("Input")
			.addFunction("GetMousePosition", &InputManager::GetMousePosition)
			.addFunction("GetMouseButton", &InputManager::GetMouseButton)
			.addFunction("GetMouseButtonUp", &InputManager::GetMouseButtomUp)
			.addFunction("GetMouseButtonDown", &InputManager::GetMouseButtonDown)
			.addFunction("GetMouseScrollDelta", &InputManager::GetMouseScollDelta)
			.addFunction("HideCursor", &LuaSupportFunctions::HideCursor)
			.addFunction("ShowCursor", &LuaSupportFunctions::ShowCursor)
			.addFunction("GetKey", &InputManager::GetKey)
			.addFunction("GetKeyDown", &InputManager::GetKeyDown)
			.addFunction("GetKeyUp", &InputManager::GetKeyUp)
			.endNamespace();
		

		luabridge::getGlobalNamespace(lua_state)
			.beginNamespace("Image")
			.addFunction("DrawUI", &RendererHandler::QueueUIDrawRequests)
			.addFunction("DrawUIEx", &RendererHandler::QueueUIEXDrawRequests)
			.addFunction("Draw", &RendererHandler::QueueWorldDrawRequests)
			.addFunction("DrawEx", &RendererHandler::QueueWorldEXDrawRequests)
			.addFunction("DrawPixel", &RendererHandler::QueuePixelDrawRequests)
			.endNamespace();

		luabridge::getGlobalNamespace(lua_state)
			.beginNamespace("Camera")
			.addFunction("SetPosition", &RendererHandler::CameraSetPosition)
			.addFunction("GetPositionX", &RendererHandler::CameraGetPositionX)
			.addFunction("GetPositionY", &RendererHandler::CameraGetPositionY)
			.addFunction("SetZoom", &RendererHandler::CameraSetZoom)
			.addFunction("GetZoom", &RendererHandler::CameraGetZoom)
			.endNamespace();
		
		// Box 2D

		luabridge::getGlobalNamespace(lua_state)
			.beginClass<b2Vec2>("Vector2")
			.addConstructor<void(*) (float,float)>()
			.addProperty("x", &b2Vec2::x)
			.addProperty("y", &b2Vec2::y)
			.addFunction("Normalize",&b2Vec2::Normalize)
			.addFunction("Length", &b2Vec2::Length)
			.addFunction("__add", &b2Vec2::operator_add)
			.addFunction("__sub", &b2Vec2::operator_sub)
			.addFunction("__mul", &b2Vec2::operator_mul)
			.endClass();

		luabridge::getGlobalNamespace(lua_state)
			.beginNamespace("Vector2")
			.addFunction("Distance", &b2Distance)
			.addFunction("Dot", static_cast<float (*)(const b2Vec2&,const b2Vec2&)>(&b2Dot))
			.endNamespace();

		Rigidbody::LuaToRigidbody(lua_state, &valid_components);
		ParticleSystem498::LuaToParticleSystem(lua_state, &valid_components);
		SpriteRenderer_C::LuaToSpriteRenderer_C(lua_state, &valid_components);


	}

	void LoadLuaFiles() 
	{
		std::string filepath = "resources/component_types/";
		std::string extension = ".lua";
		// TODO confirm filepath exists
		if (!std::filesystem::exists(filepath))
		{
			//gameEngineUtils::HandlePrint_PV("no component filepath", true);
			return;
		}

		for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(filepath)) 
		{
			if (entry.is_regular_file() && entry.path().extension() == extension) 
			{
				if (luaL_dofile(lua_state, entry.path().string().c_str()) != LUA_OK) 
				{
					std::cout << "problem with lua file " << entry.path().stem().string();
					exit(0);
				}
				valid_components[entry.path().stem().string()] = entry.path().stem().string();
			}
			else 
			{
				std::cout << "lua file: " + entry.path().string() + " -- cannot be read in" << std::endl;
			}
		}

		//std::cout << BuildInComponents::Rigidbody_Dec << std::endl;
		// Load Built In Lua Files
		// Rigidbody

		



	}





};


#endif // !LUA_HANDLER_H