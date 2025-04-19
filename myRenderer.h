#pragma once
#include <iostream>
#include <string>

#include "MapHelper.h"
#include "game.h"

#include "SDL2/SDL.h"
#include "Helper.h"


class myRenderer
{
public:

	SDL_Window* window;
	SDL_Renderer* renderer;

	
	void LoadRenderer(std::string WindowTitle,int X_res, int Y_res, Uint8 clear_color_r, Uint8 clear_color_g, Uint8 clear_color_b)
	{
		
		window = Helper::SDL_CreateWindow("Hello World", 100, 100, X_res, Y_res, SDL_WINDOW_SHOWN);
		renderer = Helper::SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

		SDL_SetRenderDrawColor(renderer, clear_color_r, clear_color_g, clear_color_b, 255);
	}

	void RenderToScreen() 
	{
	
	
		Helper::SDL_RenderPresent(renderer);
	}

};

