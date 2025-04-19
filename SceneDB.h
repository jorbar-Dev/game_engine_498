#pragma once
#include <string>
#include <filesystem>
#include "gameEngineUtils.h"
#include <vector>
#include "MapHelper.h"
#include "Actor_C.h"
#include <unordered_map>
#include "TemplateDB.h"
#include "ImageDB.h"
#include <optional>
#include "LuaHandler.h"
#include "collision.h"


class SceneDB
{
private:
	ImageDB* myImageDB;
	LuaHandler* myLuaHandler;
	int newID = 0;
	


public:
	std::map<int, Actor_C*> actors_need_lifecycle_update;
	int numComponentsAdded = 0;
	std::vector<Actor_C*> myActors;
	int PlayerIndex = -1;
	TemplateDB myTemplateDB;

	inline static std::vector<int> deadactors;

	inline static std::map<int, Actor_C*> myActorsNeedLifecycleUpdatesFirstRun_Static;

	inline static std::vector<Actor_C*>* myActors_Static;
	inline static std::map<int, Actor_C*>* myActorsNeedLifecycleUpdates_Static;
	inline static LuaHandler* myLuaHandler_Static;
	inline static int* numComponentsAdded_Static;
	inline static int* newID_Static;
	inline static int* playerIndex_Static;
	inline static ImageDB* myImageDB_Static;
	inline static TemplateDB* myTemplateDB_Static;
	inline static std::string current_scene;
	inline static std::string nextSceneToLoad;

	inline static std::unordered_map<std::string, std::vector<Actor_C*>> ActorsByName;
	inline static std::unordered_map<int, Actor_C*> ActorsByID;

	static void SceneLoadCall(std::string scene_name) 
	{
		nextSceneToLoad = scene_name;
	}

	static std::string GetSceneCall()
	{
		return current_scene;
	}
	static luabridge::LuaRef GetActorByName(std::string name)
	{

		if (ActorsByName.count(name) > 0 )
		{
			for (int i = 0; i < ActorsByName[name].size(); i++)
			{
				Actor_C* ptr = ActorsByName[name][i];
			
				if (ptr != NULL && (ptr->Alive) )
				{
					return luabridge::LuaRef(myLuaHandler_Static->GetLuaState(), ptr);
				}

			}
		}
		
		
		return luabridge::LuaRef(myLuaHandler_Static->GetLuaState());
	}

	static luabridge::LuaRef GetActorsByName(std::string name)
	{
		
		// lua_newtable(myLuaHandler_Static->GetLuaState());
		luabridge::LuaRef table = luabridge::LuaRef(luabridge::newTable(myLuaHandler_Static->GetLuaState()));
		int j = 1;
		if (ActorsByName.count(name) > 0)
		{
			for (int i = 0; i < ActorsByName[name].size(); i++)
			{
				Actor_C* ptr = ActorsByName[name][i];

				if (ptr != NULL && (ptr->Alive))
				{
					table[j] = luabridge::LuaRef(myLuaHandler_Static->GetLuaState(), ptr);
					j++;
				}

			}
		}
		

		return table;
		
	}

	static luabridge::LuaRef InstantiateActor(std::string template_name) 
	{
		// Actor_C(LuaHandler* , TemplateDB* , std::string , int , int , int* , std::vector<Actor_C*>* ) -- 
		int i = myActors_Static->size();
		
		myActors_Static->emplace_back(new Actor_C( myLuaHandler_Static, myTemplateDB_Static, template_name, *newID_Static, i, numComponentsAdded_Static, &myActorsNeedLifecycleUpdatesFirstRun_Static));
		ActorsByID[myActors_Static->back()->ID] = myActors_Static->back();
		ActorsByName[myActors_Static->back()->actor_name].push_back(myActors_Static->back());
		(*newID_Static) += 1;
		return luabridge::LuaRef(myLuaHandler_Static->GetLuaState(), myActors_Static->back());
		
	}

	static void RemoveActor(luabridge::LuaRef actor)
	{
		Actor_C* ptr = actor.cast<Actor_C*>();
		if (ptr != NULL && ptr->Index < myActors_Static->size() && ptr == (*myActors_Static)[ptr->Index])
		{
			int i = ptr->Index;
			(*myActors_Static)[i]->Destroy();
			deadactors.push_back(i);
			return;
		}
		for (int i = 0; i < myActors_Static->size(); i++) 
		{
			if ((*myActors_Static)[i] == ptr) 
			{
				(*myActors_Static)[i]->Destroy();
				deadactors.push_back(i);
				return;
			}
		}
	}

	static void MarkActorAsDoNotDestroyCall(luabridge::LuaRef actor) 
	{
		Actor_C* ptr = actor.cast<Actor_C*>();
		ptr->DoNotDestroyOnLoad = true;
	}

	//need actors
	// maybe sort based on updates
	// move if they dont move they can probably be skipped
	//// secondary list for requires update
	////  

	


	
	void LoadSceneDB(ImageDB* imageDB, LuaHandler* luaHandler)
	{
		myImageDB = imageDB;
		myLuaHandler = luaHandler;
		myTemplateDB.LoadTemplateDB(luaHandler);
		
		SceneDB::myActors_Static = &myActors;
		SceneDB::myActorsNeedLifecycleUpdates_Static = &actors_need_lifecycle_update;
		SceneDB::numComponentsAdded_Static = &numComponentsAdded;
		SceneDB::newID_Static = &newID;
		SceneDB::myLuaHandler_Static = luaHandler;
		SceneDB::myImageDB_Static = myImageDB;
		SceneDB::myTemplateDB_Static = &myTemplateDB;
		SceneDB::playerIndex_Static = &PlayerIndex;


		// Update Lua
		luabridge::getGlobalNamespace(luaHandler->GetLuaState())
			.beginClass<Actor_C>("Actor")
			.addFunction("GetName", &Actor_C::GetName)
			.addFunction("GetID", &Actor_C::GetID)
			.addFunction("GetComponentByKey", &Actor_C::GetComponentByKey)
			.addFunction("GetComponent", &Actor_C::GetComponent)
			.addFunction("GetComponents", &Actor_C::GetComponents)
			.addFunction("AddComponent", &Actor_C::AddComponent)
			.addFunction("RemoveComponent", &Actor_C::RemoveComponent)
			.endClass();
		
		luabridge::getGlobalNamespace(luaHandler->GetLuaState())
			.beginNamespace("Actor")
			.addFunction("Find", &GetActorByName)
			.addFunction("FindAll", &GetActorsByName)
			.addFunction("Instantiate", &InstantiateActor)
			.addFunction("Destroy", &RemoveActor)
			.endNamespace();

		luabridge::getGlobalNamespace(luaHandler->GetLuaState())
			.beginNamespace("Scene")
			.addFunction("GetCurrent", &GetSceneCall)
			.addFunction("DontDestroy", &MarkActorAsDoNotDestroyCall)
			.addFunction("Load", &SceneLoadCall)
			.endNamespace();

		collision::LuaToCollision(luaHandler->GetLuaState());


			

	}

	static void checkIfSceneExists(std::string SceneName)
	{
		std::string tmp = "resources/scenes/" + SceneName + ".scene";
		if (!std::filesystem::exists(tmp))
		{
			gameEngineUtils::HandlePrint_PV("error: scene " + SceneName + " is missing", true);
			exit(0);
		}
	}

	void LoadScene(std::string SceneName)
	{
		checkIfSceneExists(SceneName);
		std::string tmp = "resources/scenes/" + SceneName + ".scene";
		current_scene = SceneName;
		UnloadCurrentScene();
		rapidjson::Document scene;
		gameEngineUtils::ReadJsonFile(tmp, scene);
		PlayerIndex = -1;
		if (scene.HasMember("actors"))
		{
			
			myActors.reserve(scene["actors"].Size());
			// load the actors in
			for (int i = 0; i < scene["actors"].Size(); i++)
			{
				std::string name = "";
				std::string template_name = "";
				
				myActors.emplace_back(new Actor_C(myLuaHandler, &myTemplateDB, &scene["actors"][i], newID, myActors.size(), &numComponentsAdded, &actors_need_lifecycle_update));
				ActorsByID[myActors.back()->ID] = myActors.back();
				ActorsByName[myActors.back()->actor_name].push_back(myActors.back());
				newID += 1;
				
			}
		}
	}

	void UnloadCurrentScene() 
	{
		std::vector<int> indicesToRemove;
		indicesToRemove.reserve(myActors.size());
		for (int i = myActors.size() - 1; i >= 0; i--) 
		{
			if (!myActors[i]->DoNotDestroyOnLoad) 
			{
				indicesToRemove.push_back(i);
			}
		}
		for (int i = 0; i < indicesToRemove.size(); i++) 
		{
			myActors[indicesToRemove[i]]->Destroy();
			myActors.erase(myActors.begin() + indicesToRemove[i]);
		}
		for (Actor_C* act : myActors)
		{
			act->UpdateLifeCycleVectors(false);
		}
	}

	void LateUpdate() 
	{
		if (deadactors.size() > 0) 
		{
			std::stable_sort(deadactors.begin(), deadactors.end());
			for (int i = SceneDB::deadactors.size() - 1; i << SceneDB::deadactors.size() >= 0; i--)
			{
			
				myActors.erase(myActors.begin() + SceneDB::deadactors[i]);
			}
			int i = deadactors.front(); // todo check this logic 
			for (; i < myActors.size(); i++) 
			{
				myActors[i]->Index = i;
			}
		}
		for(auto & act : actors_need_lifecycle_update)
		{
			act.second->UpdateLifeCycleVectors(false);
		}
		for (auto & act : SceneDB::myActorsNeedLifecycleUpdatesFirstRun_Static)
		{
			act.second->UpdateLifeCycleVectors(true);
		}
		SceneDB::deadactors.clear();
		actors_need_lifecycle_update.clear();
		SceneDB::myActorsNeedLifecycleUpdatesFirstRun_Static.clear();

		if (nextSceneToLoad != "") 
		{
			LoadScene(nextSceneToLoad);
			nextSceneToLoad = "";
			

			if (deadactors.size() > 0)
			{
				std::stable_sort(deadactors.begin(), deadactors.end());
				for (int i = SceneDB::deadactors.size() - 1; i << SceneDB::deadactors.size() >= 0; i--)
				{

					myActors.erase(myActors.begin() + SceneDB::deadactors[i]);
				}
				int i = deadactors.front();
				for (; i < myActors.size(); i++)
				{
					myActors[i]->Index = i;
				}
			}
			for (auto & act : actors_need_lifecycle_update)
			{
				act.second->UpdateLifeCycleVectors(false);
			}
			for (auto & act : SceneDB::myActorsNeedLifecycleUpdatesFirstRun_Static)
			{
				act.second->UpdateLifeCycleVectors(true);
			}
			SceneDB::deadactors.clear();
			actors_need_lifecycle_update.clear();
			SceneDB::myActorsNeedLifecycleUpdatesFirstRun_Static.clear();

		}
	}


};

