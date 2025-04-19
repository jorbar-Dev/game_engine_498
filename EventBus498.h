#pragma once


#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <string>
#include <vector>
#include <unordered_map>
class SubscriptionData 
{
public:
	std::string event_type;
	luabridge::LuaRef function = NULL;
	luabridge::LuaRef component = NULL;
	
	
};
class EventBus498
{
public:

	static inline void StartUp(lua_State* lua_s) 
	{
		state = lua_s;
		luabridge::getGlobalNamespace(state)
			.beginNamespace("Event")
			.addFunction("Publish", &EventBus498::Publish)
			.addFunction("Subscribe", &EventBus498::Subscribe)
			.addFunction("Unsubscribe", &EventBus498::UnSubscribe)
			.endNamespace();
	}

	static inline lua_State* state;
	static inline std::unordered_map < std::string, std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>>> events;
	static inline std::vector<SubscriptionData> Subscribes;
	static inline std::vector<SubscriptionData> UnSubscribes;
	
	static inline void Subscribe(std::string event_t, luabridge::LuaRef comp, luabridge::LuaRef func)
	{
		SubscriptionData tmp;
		tmp.event_type = event_t;
		tmp.component = comp;
		tmp.function = func;

		Subscribes.emplace_back(tmp);
	}
	// hello there linux
	static inline void UnSubscribe(std::string event_t, luabridge::LuaRef comp, luabridge::LuaRef func)
	{
		SubscriptionData tmp;
		tmp.event_type = event_t;
		tmp.component = comp;
		tmp.function = func;

		UnSubscribes.emplace_back(tmp);
	}

	static inline void Publish(std::string event_type,luabridge::LuaRef ptr)
	{
		if (events.count(event_type) == 0) 
		{
			return;
		}
		std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>>& event_list = events[event_type];
		for (int i = 0; i < event_list.size(); i++) 
		{

			luabridge::LuaRef func = (event_list[i]).first; // function
			luabridge::LuaRef comp = (event_list[i]).second;
			
			//bool enabled = (*comp)["enabled"].cast<bool>();

			try
			{
				if (true)
				{
					(func)(comp,ptr);
				}
			}
			catch (const luabridge::LuaException& e)
			{
				// TODO: FIGURE OUT HOW TO GET THIS IN
				std::string error_message = e.what();
				std::replace(error_message.begin(), error_message.end(), '\\', '/');
				std::cout << "\033[31m" << "unknown actor" << " : " << error_message << "\033[0m" << std::endl; // func["actor"].actor_name
			}

		}
	}

	static inline void EventLateUpdate() 
	{
		for (int i = 0; i < Subscribes.size(); i++) 
		{
			std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>>& event_list = events[Subscribes[i].event_type];
			bool is_unscribed = true;
			for (int j = 0; j < event_list.size(); j++)
			{

				luabridge::LuaRef func = event_list[j].first; // function
				if (event_list[j].first == Subscribes[i].function && event_list[j].second == Subscribes[i].component)
				{
					is_unscribed = false;
					break;
				}

			}
			if (is_unscribed) 
			{
				event_list.emplace_back(Subscribes[i].function, Subscribes[i].component);
			}

		}
		for (int i = 0; i < UnSubscribes.size(); i++)
		{
			std::vector<std::pair<luabridge::LuaRef, luabridge::LuaRef>>& event_list = events[UnSubscribes[i].event_type];
			bool is_unscribed = true;
			for (int i = 0; i < event_list.size(); i++)
			{

				luabridge::LuaRef func = event_list[i].first; // function
				if (event_list[i].first == UnSubscribes[i].function && event_list[i].second == UnSubscribes[i].component)
				{
					event_list.erase(event_list.begin() + i);
					break;
				}

			}
			

		}
		Subscribes.clear();
		UnSubscribes.clear();
	
	}
};

