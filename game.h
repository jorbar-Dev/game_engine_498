#pragma once
#include <iostream>
#include <string>
#include "glm/glm.hpp"
#include <iostream>
#include "gameEngineUtils.h"
#include <filesystem>
#include "SceneDB.h"
#include "Actor_C.h"
#include "RendererHandler.h"
#include "Helper.h"
#include "ImageDB.h"
#include "TextDB.h"
#include "Helper.h"
#include "RendererHandler.h"
#include "AudioDB.h"
#include <unordered_map>
#include "LuaHandler.h"
#include "InputManager.h"
#include "ContactListener498.h"
#include "Rigidbody.h"
#include "EventBus498.h"

class game
{
	// hw7 mac 8
	bool LoadingNewSceneFlag = false;
	SDL_Event inputE;

	bool Up_Arrow = false;
	bool Down_Arrow = false;
	bool Right_Arrow = false;
	bool Left_Arrow = false;

public:

	bool running = true;
	bool quit = false;
	rapidjson::Document gameConfig;
	rapidjson::Document renderingConfig;

	std::string game_title = "";
	// scene management
	SceneDB mySceneDB;

	// image management
	ImageDB myImageDB;
	TextDB myTextDB;



	// rendering 2
	int width_set = 640;
	int height_set = 360;
	float zoom_factor = 1.0f;
	float cam_ease_factor = 1.0f;

	RendererHandler myRendererHandler;
	Uint8 start_r = 255;
	Uint8 start_g = 255;
	Uint8 start_b = 255;

	int exitNum = 0;

	AudioDB myAudioDB;
	
	LuaHandler myLuaHandler;
	//UI
	// Heart Image

	// gameplay
	// linux

	void RunGame() 
	{
		startGame();
		mySceneDB.LateUpdate();
		//myLuaHandler.OnStart();
		while (running)
		{
			ReadInput();
			myLuaHandler.OnStart();
			myLuaHandler.OnUpdate();
			myLuaHandler.OnLateUpdate();
			EventBus498::EventLateUpdate();
			if (Rigidbody::physicsWorld != NULL) 
			{
				Rigidbody::physicsWorld->Step(1.0f / 60.0f, 8, 3);
			}

			// Rendering
			RenderUpdate();

			//HandleRender(); //
			if (quit)
			{
				std::cout << "leaving running loop" << std::endl;
				break;
				// exit(exitNum);
			}


			InputManager::LateUpdate();
			mySceneDB.LateUpdate();
		}
	}
private:

	void startGame()
	{
		if (!std::filesystem::exists("resources")) 
		{
			gameEngineUtils::HandlePrint_PV("error: resources/ missing", true);
			exit(exitNum);
		}
		if (!std::filesystem::exists("resources/game.config"))
		{
			gameEngineUtils::HandlePrint_PV("error: resources/game.config missing", true);
			exit(exitNum);
		}
		gameEngineUtils::ReadJsonFile("resources/game.config", gameConfig);
		if (!gameConfig.HasMember("initial_scene"))
		{
			// TODO check to see if I should be checking if initial scene is undefined
			gameEngineUtils::HandlePrint_PV("error: initial_scene unspecified", true);
			exit(exitNum);

		}
		
		
		

		// start up
		
		// read in data for rendering
		if (std::filesystem::exists("resources/rendering.config"))
		{
			gameEngineUtils::ReadJsonFile("resources/rendering.config", renderingConfig);
			if (renderingConfig.HasMember("x_resolution"))
			{
				width_set = renderingConfig["x_resolution"].GetInt();
			}
			if (renderingConfig.HasMember("y_resolution"))
			{
				height_set = renderingConfig["y_resolution"].GetInt();
			}
			if (renderingConfig.HasMember("clear_color_r"))
			{
				start_r = renderingConfig["clear_color_r"].GetUint();
			}
			if (renderingConfig.HasMember("clear_color_g"))
			{
				start_g = renderingConfig["clear_color_g"].GetUint();
			}
			if (renderingConfig.HasMember("clear_color_b"))
			{
				start_b = renderingConfig["clear_color_b"].GetUint();
			}
			if (renderingConfig.HasMember("zoom_factor"))
			{
				zoom_factor = renderingConfig["zoom_factor"].GetFloat();
			}
			if (renderingConfig.HasMember("cam_ease_factor"))
			{
				cam_ease_factor = renderingConfig["cam_ease_factor"].GetFloat();
			}
			
		}
		
		InputManager::Init();
		myLuaHandler.LoadLuaHandler();
		Rigidbody::contact_listener = new ContactListener498();
		EventBus498::StartUp(myLuaHandler.GetLuaState());
		myRendererHandler.LoadRenderer(game_title, width_set, height_set,start_r,start_g,start_b,cam_ease_factor);
		myImageDB.LoadImageDB(myRendererHandler.renderer);
		myAudioDB.LoadAudioManager(&myLuaHandler);
		mySceneDB.LoadSceneDB(&myImageDB,&myLuaHandler);
		mySceneDB.LoadScene(gameConfig["initial_scene"].GetString());
		myTextDB.InitTextDB(myLuaHandler.GetLuaState());
		
		running = true;
	}

	bool running_gameplay_music = false;
	bool check_for_game_audio = true;

	
	inline void ReadInput() 
	{
		while (Helper::SDL_PollEvent(&inputE))
		{
			if (inputE.type == SDL_QUIT)
			{
				quit = true;
			}
			InputManager::ProcessEvent(inputE);
			
		}

	}

	inline void RenderUpdate() 
	{
		//myRendererHandler.UpdateCamPosition();
		//myRendererHandler.UpdateScale(zoom_factor,zoom_factor);
		//HandleRenderSDL();
		//myRendererHandler.UpdateScale(1,1);
		//RenderUI();
		RendererHandler::UpdateStoredSprites();
		myRendererHandler.WorldDrawUpdate();
		myRendererHandler.UIDrawUpdate();
		myRendererHandler.TextDrawUpdate();
		myRendererHandler.PixelDrawUpdate();
		if (LoadingNewSceneFlag) 
		{
			LoadingNewSceneFlag = false;
			return;
		}

		myRendererHandler.RenderToScreen();

	}

	




};
