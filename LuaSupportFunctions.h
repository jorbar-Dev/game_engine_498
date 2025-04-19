#pragma once
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include "Helper.h"
#include <thread>



class LuaSupportFunctions
{
public:
	static void TestFunc(const std::string& val)
	{
		std::cout << val << std::endl;
	}

	static void DebugLog(const std::string& message)
	{
		std::cout << message << std::endl;
	}

	static void DebugLogError(const std::string& message)
	{
		std::cout << message << std::endl;
	}
	
	static void QuitApplication() 
	{
		exit(0);
	}

	static void ApplicationSleep(float milliseconds) 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds((long long)(milliseconds)));
	}

	static int GetFrameNumber() 
	{
		return Helper::GetFrameNumber();
	}

	static void HideCursor() 
	{
		SDL_ShowCursor(false);
	}

	static void ShowCursor() 
	{
		SDL_ShowCursor(true);
	}

	static void OpenURL(std::string url) 
	{
#ifdef _WIN32
		std::system(("start " + url).c_str());
#elif __APPLE__
		std::system(("open " + url).c_str());
#else 
		std::system(("xdg-open " + url).c_str());
#endif // _WIN32

	}



};

