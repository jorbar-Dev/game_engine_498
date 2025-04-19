#pragma once

#include <string>
#include <filesystem>
#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"
#include "Helper.h"
#include "gameEngineUtils.h"
// #include "RendererHandler.h"
#include "unordered_map"
#include "queue"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"


//#include "LuaHandler.h"
class TextDB
{

public:
	static inline std::unordered_map<std::string, std::unordered_map<int, TTF_Font*>> myFonts;
	void InitTextDB(lua_State* luaHandler)
	{

		
	}

	static inline TTF_Font* GetFont(std::string font_name, int font_size)
	{
		TTF_Font* font;

		if (TextDB::myFonts.count(font_name) > 0 && TextDB::myFonts[font_name].count(font_size) > 0)
		{
			return TextDB::myFonts[font_name][font_size];
		}

		std::string filename = "resources/fonts/" + font_name + ".ttf";
		if (std::filesystem::exists(filename))
		{
			font = TTF_OpenFont(filename.c_str(), font_size);
		}
		else
		{
			gameEngineUtils::HandlePrint_PV("error: font " + font_name + " missing", true);
			exit(0);
		}
		TextDB::myFonts[font_name][font_size] = font;
		return font;
	}
	

	



};

