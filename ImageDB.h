#pragma once

#include <unordered_map>
#include <filesystem>
#include "gameEngineUtils.h"
#include <string>
#include "SDL2_image/SDL_image.h"
#include "SDL2/SDL.h"
#include "Helper.h"

class ImageDB
{

	// comment
	static inline std::unordered_map<std::string, SDL_Texture*> myTextures;
	static inline SDL_Renderer* rend;
public:
	void LoadImageDB(SDL_Renderer* Rend) 
	{
		rend = Rend;
		LoadImageFiles();
	}

	static SDL_Texture* LoadImage(std::string image_name)
	{
		if (ImageDB::myTextures.count(image_name) > 0) 
		{
			return ImageDB::myTextures[image_name];
		}
		else 
		{
			gameEngineUtils::HandlePrint_PV("error: missing image " + image_name, true);
			exit(0);
		}
	}

	void LoadImageFiles()
	{
		std::string filepath = "resources/images/";
		std::string extension = ".png";
		// TODO confirm filepath exists
		if (!std::filesystem::exists(filepath))
		{
			return;
		}

		for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(filepath))
		{
			if (entry.is_regular_file() && entry.path().extension() == extension)
			{
				ImageDB::myTextures[entry.path().stem().string()] = IMG_LoadTexture(rend, (entry).path().string().c_str());
			}
		}
	}

	static void CreateDefaultParticleTextureWithName(const std::string& name) 
	{
		if (ImageDB::myTextures.count(name) > 0) 
		{
			return;
		}

		// create cpu surface
		SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, 8, 8, 32, SDL_PIXELFORMAT_RGBA8888);
		
		// ensure white color
		Uint32 white_color = SDL_MapRGBA(surface->format, 255, 255, 255, 255);
		SDL_FillRect(surface, NULL, white_color);

		// create gpu texture from cpu surface now that we are done editing it
		SDL_Texture* texture = SDL_CreateTextureFromSurface(ImageDB::rend, surface);

		// clean up surface and cache it for later use
		SDL_FreeSurface(surface);
		ImageDB::myTextures[name] = texture;
	}
};

