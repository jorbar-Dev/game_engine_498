#pragma once

#include <string>

#include "SDL2/SDL.h"
#include "Helper.h"
#include <algorithm>
#include <optional>
#include "queue"
#include "ImageDB.h"
#include "glm/glm.hpp"
#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"
#include "box2d/box2d.h"
#include "utility"
#include "map"
#include "queue"
#include "TextDB.h"

struct SpriteData {
	int layer;
	int index;
	bool UI;
};

struct LayerObject {

	std::queue<int> freed_index;

	std::vector<bool> alive;
	std::vector<SDL_Texture*> textures;
	std::vector<SDL_FRect> srcs;
	std::vector<SDL_FRect> dess;
	std::vector<SDL_Color> colors;
	std::vector<SDL_FPoint> centers;
	std::vector<float> rot_degrees;
	std::vector<float> scale_xs;
	std::vector<float> scale_ys;
	std::vector<float> center_xs;
	std::vector<float> center_ys;
	std::vector<SDL_RendererFlip> flips;
	std::vector<int> sorting_order;
	bool do_sort;
	LayerObject() 
	{
		do_sort = true;
	}
};

struct LayerDrawRequest {
	int sorting_order;
	SDL_Texture* text;
	SDL_FRect src;
	SDL_FRect des;
	SDL_FPoint center;
	SDL_Color color;
	float rotation;
	SDL_RendererFlip flip;
	LayerDrawRequest( SDL_Texture* Text, SDL_FRect Src, SDL_FRect Des, SDL_FPoint Center, SDL_Color col, float rot, SDL_RendererFlip Flip, int Sorting_order )
	{
		text = Text;
		src = Src;
		des = Des;
		center = Center;
		color = col;
		rotation = rot;
		flip = Flip;
		sorting_order = Sorting_order;

	}

	bool operator<(const  LayerDrawRequest& other)  const
	{
		return sorting_order < other.sorting_order;
	}
};

struct textDrawRequest
{
	std::string content;
	int x, y, r, g, b, a;
	TTF_Font* font;

	textDrawRequest(std::string str, int X, int Y, int R, int G, int B, int A, TTF_Font* Font) 
	{
		content = str;
		x = X;
		y = Y;
		r = R;
		g = G;
		b = B;
		a = A;
		font = Font;
	}
};

struct imageDrawRequest
{
	SDL_Texture* image;
	float y;
	float x;
	int rotationDegrees;
	float scaleX;
	float scaleY;
	float pivotX;
	float pivotY;
	int r;
	int g;
	int b;
	int a;
	int sortingOrder;

	imageDrawRequest(SDL_Texture* Image, float X, float Y, int rot, float Sx, float Sy, float Px, float Py, int R, int G, int B, int A, int Sorder )
	{
		image = Image;
		x = X;
		y = Y;
		rotationDegrees = rot;
		scaleX = Sx;
		scaleY = Sy;
		pivotX = Px;
		pivotY = Py;
		r = R;
		g = G;
		b = B;
		a = A;
		sortingOrder = Sorder;

	}

};

class RendererHandler
{

public:

	static inline std::queue<textDrawRequest> textDrawRequests;
	static inline std::vector<imageDrawRequest> uiDrawRequests;
	static inline std::vector<imageDrawRequest> worldDrawRequests;
	static inline std::queue<imageDrawRequest> pixelDrawRequests;
	static inline RendererHandler* me;

	static inline float width_r;
	static inline float height_r;

	static inline SDL_Window* window;
	static inline SDL_Renderer* renderer;
	static inline Uint8 start_clear_r = 255;
	static inline Uint8 start_clear_g = 255;
	static inline Uint8 start_clear_b = 255;
	static inline glm::vec2* desired_pos;
	static inline glm::vec2 current_pos = glm::vec2(0, 0);

	static inline float cam_offset_x;
	static inline float cam_offset_y;

	static inline float ZoomFactor;
	static inline float x_scale;
	static inline float y_scale;

	static inline float cam_ease_fact;

	static void CameraSetPosition(float x, float y)
	{
		current_pos = { x,y };
	}
	static void CameraSetZoom(float zoom)
	{
		ZoomFactor = zoom;
	}
	static float CameraGetPositionX() 
	{
		return current_pos.x;
	}
	static float CameraGetPositionY()
	{
		return current_pos.y;
	}
	static float CameraGetZoom()
	{
		return ZoomFactor;
	}

	static void MakeDrawTextRequest(std::string content, float x, float y, std::string font_name, float font_size, float r, float g, float b, float a)
	{
		TTF_Font* font = TextDB::GetFont(font_name, font_size);
		QueueTextDrawRequests(content, x, y, r, g, b, a, font);
	}

	static void QueueTextDrawRequests(std::string str, int X, int Y, int R, int G, int B, int A, TTF_Font*& Font) 
	{
		RendererHandler::textDrawRequests.emplace(str, X, Y, R, G, B, A, Font);
	}
	static void QueueUIDrawRequests(std::string image, float X, float Y)
	{

		RendererHandler::uiDrawRequests.emplace_back(ImageDB::LoadImage(image),X,Y,0,1,1,0,0,255,255,255,255,0);
	}
	static void QueueUIEXDrawRequests(std::string image, float X, float Y, float R, float G, float B, float A, float Order)
	{

		RendererHandler::uiDrawRequests.emplace_back(ImageDB::LoadImage(image), X, Y, 0, 1, 1, 0, 0, R, G, B, A, Order);
	}
	static void QueueWorldDrawRequests(std::string image, float X, float Y)
	{

		RendererHandler::worldDrawRequests.emplace_back(ImageDB::LoadImage(image), X, Y, 0, 1, 1, 0.5f, 0.5f, 255, 255, 255, 255, 0);
	}
	static void QueueWorldEXDrawRequests(std::string image, float X, float Y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float R, float G, float B, float A, float Order)
	{
		RendererHandler::worldDrawRequests.emplace_back(ImageDB::LoadImage(image), X, Y, (int)rotation_degrees, scale_x, scale_y, pivot_x, pivot_y, R, G, B, A, Order);
	}
	static void QueuePixelDrawRequests(float X, float Y, float R, float G, float B, float A)
	{
		SDL_Texture* ptr = NULL;
		RendererHandler::pixelDrawRequests.emplace(ptr, X, Y, 0, 1, 1, 0, 0, R, G, B, A, 0);
	}
	void TextDrawUpdate() 
	{
		while (!RendererHandler::textDrawRequests.empty())
		{
			DrawText(RendererHandler::textDrawRequests.front());
			RendererHandler::textDrawRequests.pop();
			// std::cout << "DrawText" << std::endl;
		}
	}
	void WorldDrawUpdate()
	{
		UpdateScale(ZoomFactor, ZoomFactor); // todo replace with desired scaled
		sortQuest(RendererHandler::worldDrawRequests);
		for (int i = 0; i < RendererHandler::worldDrawRequests.size(); i++)
		{
			DrawWorld(RendererHandler::worldDrawRequests[i]);
		}
		RendererHandler::worldDrawRequests.clear();
		UpdateScale(1, 1); // todo replace with desired scaled
	}
	void UIDrawUpdate()
	{
		sortQuest(RendererHandler::uiDrawRequests);
		for (int i = 0; i < uiDrawRequests.size(); i++) 
		{
			
			DrawUI(uiDrawRequests[i]);
		}
		uiDrawRequests.clear();
	}
	void PixelDrawUpdate()
	{
		while (!RendererHandler::pixelDrawRequests.empty())
		{
			DrawPixel(RendererHandler::pixelDrawRequests.front());
			RendererHandler::pixelDrawRequests.pop();
			// std::cout << "DrawText" << std::endl;
		}
	}
	inline void UpdateCamPosition() 
	{
		if (desired_pos == NULL) 
		{
			return;
		}
		current_pos = glm::mix(current_pos, *desired_pos, cam_ease_fact);
	}
	void LoadRenderer(std::string WindowTitle, int X_res, int Y_res, Uint8 clear_color_r, Uint8 clear_color_g, Uint8 clear_color_b, float cam_ease_factor)
	{
		RendererHandler::start_clear_b = clear_color_b;
		RendererHandler::start_clear_g = clear_color_g;
		RendererHandler::start_clear_r = clear_color_r;
		RendererHandler::width_r = X_res;
		RendererHandler::height_r = Y_res;
		RendererHandler::cam_ease_fact = cam_ease_factor;
		RendererHandler::x_scale = 1;
		RendererHandler::y_scale = 1;
		RendererHandler::window = Helper::SDL_CreateWindow("Hello World", 100, 100, X_res, Y_res, SDL_WINDOW_SHOWN);
		RendererHandler::renderer = Helper::SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
		RendererHandler::ZoomFactor = 1.0f;

		SDL_SetRenderDrawColor(renderer, clear_color_r, clear_color_g, clear_color_b, 255);
		SDL_RenderClear(renderer);
		
		RendererHandler::me = this;
	}
	static void inline DrawText( textDrawRequest & request) 
	{
		SDL_Color col;
		col.r = request.r;
		col.g = request.g;
		col.b = request.b;
		col.a = request.a;
		SDL_Surface* mySurf = TTF_RenderText_Solid(request.font, request.content.c_str(), col);
		SDL_Texture* myText = SDL_CreateTextureFromSurface(me->renderer, mySurf);
		SDL_FPoint center;

		center.y = 0;
		center.x = 0;


		SDL_FRect src;
		src.h = 0;
		src.w = 0;
		src.x = 0;
		src.y = 0;

		Helper::SDL_QueryTexture(myText, &src.w, &src.h);

		SDL_FRect des = src;
		des.h = src.h;
		des.w = src.w;
		des.x = request.x;
		des.y = request.y;
		Helper::SDL_RenderCopyEx(0, "", me->renderer, myText, &src, &des, 0, &center, SDL_FLIP_NONE);
	}
	static void inline DrawWorld(imageDrawRequest& request) 
	{
		SDL_Color col;
		col.r = request.r;
		col.g = request.g;
		col.b = request.b;
		col.a = request.a;
		request.x = request.x - current_pos.x;
		request.y = request.y - current_pos.y;
		SDL_Texture* textToUse = request.image;
		

		SDL_FRect src = {0,0,0,0};
		
		Helper::SDL_QueryTexture(textToUse, &src.w, &src.h);

		SDL_FRect des = src;

		des.w = src.w * glm::abs(request.scaleX);
		des.h = src.h * glm::abs(request.scaleY);
		SDL_FPoint center = { (request.pivotX * src.w * glm::abs(request.scaleX)),(request.pivotY * src.h * glm::abs(request.scaleY)) };
		des.x = ((request.x * 100) + width_r * 0.5f - center.x );
		des.y = ((request.y * 100) + height_r * 0.5f - center.y );

		SDL_RendererFlip flip = SDL_FLIP_NONE;
		if (request.scaleY < 0)
		{
			flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | flip);
		}
		if (request.scaleX < 0)
		{
			flip = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | flip);
		}
		if (!gameEngineUtils::isOverlapping(src.w, src.h, src.x, src.y, width_r, height_r, 0, 0, false))
		{
			return;
		}


		SDL_SetTextureColorMod(request.image, col.r, col.g, col.b);
		SDL_SetTextureAlphaMod(request.image, col.a);
		Helper::SDL_RenderCopyEx(0,"",renderer, textToUse, &src, &des, request.rotationDegrees, &center, flip);
		SDL_SetTextureColorMod(request.image, 255, 255, 255);
		SDL_SetTextureAlphaMod(request.image, 255);



	
	}
	static void inline DrawUI( imageDrawRequest& request) 
	{
		SDL_Color col;
		col.r = request.r;
		col.g = request.g;
		col.b = request.b;
		col.a = request.a;
		SDL_FPoint center;

		center.y = 0;
		center.x = 0;


		SDL_FRect src;
		src.h = 0;
		src.w = 0;
		src.x = 0;
		src.y = 0;

		Helper::SDL_QueryTexture(request.image, &src.w, &src.h);

		SDL_FRect des = src;
		des.h = src.h;
		des.w = src.w;
		des.x = request.x;
		des.y = request.y;
		SDL_SetTextureColorMod(request.image, col.r, col.g, col.b);
		SDL_SetTextureAlphaMod(request.image, col.a);
		Helper::SDL_RenderCopyEx(0, "", me->renderer, request.image, &src, &des, 0, &center, SDL_FLIP_NONE);
		SDL_SetTextureColorMod(request.image, 255, 255, 255);
		SDL_SetTextureAlphaMod(request.image, 255);
	
	}
	static void inline DrawPixel( imageDrawRequest& request) 
	{
		SDL_Color col;
		col.r = request.r;
		col.g = request.g;
		col.b = request.b;
		col.a = request.a;


		SDL_SetRenderDrawBlendMode(RendererHandler::renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(RendererHandler::renderer, col.r, col.g, col.b, col.a);
		SDL_RenderDrawPoint(RendererHandler::renderer, (int)request.x, (int)request.y);
		SDL_SetRenderDrawColor(RendererHandler::renderer, RendererHandler::start_clear_r, RendererHandler::start_clear_g, RendererHandler::start_clear_b, 255);


		SDL_SetRenderDrawBlendMode(RendererHandler::renderer, SDL_BLENDMODE_NONE);



	}
	inline void RenderToScreen()
	{
		Helper::SDL_RenderPresent(RendererHandler::renderer);
		SDL_SetRenderDrawColor(RendererHandler::renderer, RendererHandler::start_clear_r, RendererHandler::start_clear_g, RendererHandler::start_clear_b, 255);
		SDL_RenderClear(RendererHandler::renderer);
	}
	inline void ClearImage() 
	{
		SDL_SetRenderDrawColor(RendererHandler::renderer, RendererHandler::start_clear_r, RendererHandler::start_clear_g, RendererHandler::start_clear_b, 255);
		SDL_RenderClear(RendererHandler::renderer);
	}
	inline void UpdateScale(float x, float y) 
	{
		SDL_RenderSetScale(RendererHandler::renderer, x, y);
		x = 1 / x;
		y = 1 / y;
		RendererHandler::width_r = RendererHandler::width_r / RendererHandler::x_scale;
		RendererHandler::height_r = RendererHandler::height_r / RendererHandler::y_scale;
		RendererHandler::width_r = RendererHandler::width_r * x;
		RendererHandler::height_r = RendererHandler::height_r * y;
		RendererHandler::x_scale = x;
		RendererHandler::y_scale = y;
	}
	inline static void inline sortQuest(std::vector<imageDrawRequest>& queue)
	{
		
		std::stable_sort(queue.begin(), queue.end(), [](imageDrawRequest first, imageDrawRequest second)
			{
				return first.sortingOrder < second.sortingOrder;
			});
		
	}
	static inline int sprite_id = -1;
	static inline std::map<int, LayerObject> world_layers;
	static inline std::map<int, LayerObject> UI_layers;
	static inline std::unordered_map<int, SpriteData> index_to_data;
	// dod systems
	static inline int ReserveSpriteSlot(SDL_Texture* text, float x, float y, float scale_x, float scale_y, float rot_degrees, float center_x, float center_y, int R, int G, int B, int A, int sorting_order, int layer, bool is_UI)
	{
		LayerObject* layer_obj_ref;
		if (is_UI) 
		{
			layer_obj_ref = &UI_layers[layer];
		}
		else
		{
			layer_obj_ref = &world_layers[layer];
		}
		int index = 0;
		if (layer_obj_ref->freed_index.empty()) // use new slot 
		{
			// save color data
			SDL_Color col = { R,G,B,A };
			layer_obj_ref->colors.emplace_back(col);
			// get image ptr
			
			layer_obj_ref->textures.emplace_back(text);
			// save src 
			SDL_FRect src = { 0,0,0,0 };
			Helper::SDL_QueryTexture(text, &src.w, &src.h);
			layer_obj_ref->srcs.emplace_back(src);
			// save fpoint
			SDL_FPoint center = { (center_x * src.w * glm::abs(scale_x)),(center_y * src.h * glm::abs(scale_y)) }; // todo add support for x vs y scaling

			layer_obj_ref->centers.emplace_back(center);
			layer_obj_ref->scale_xs.emplace_back(scale_x);
			layer_obj_ref->scale_ys.emplace_back(scale_y);
			layer_obj_ref->center_xs.emplace_back(center_x);
			layer_obj_ref->center_ys.emplace_back(center_y);
			// save flip
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (scale_x < 0)
			{
				flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | flip);
			}
			if (scale_y < 0)
			{
				flip = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | flip);
			}
			layer_obj_ref->flips.emplace_back(flip);
			// save empty des
			src.w *= scale_x;
			src.h *= scale_y;
			src.x = x;
			src.y = y;
			layer_obj_ref->dess.emplace_back(src);

			layer_obj_ref->rot_degrees.emplace_back(rot_degrees);
			// save alive
			layer_obj_ref->alive.emplace_back(true);
			// save scale
			index = layer_obj_ref->alive.size()-1;

			layer_obj_ref->sorting_order.emplace_back(sorting_order);


		}
		else // reuse old slot 
		{
			index = layer_obj_ref->freed_index.front();
			layer_obj_ref->freed_index.pop();

			// save color data
			SDL_Color col = { R,G,B,A };
			layer_obj_ref->colors[index] = (col);
			// get image ptr
			layer_obj_ref->textures[index] = (text);
			// save src 
			SDL_FRect src = { 0,0,0,0 };
			Helper::SDL_QueryTexture(text, &src.w, &src.h);
			layer_obj_ref->srcs[index] = (src);
			// save fpoint
			SDL_FPoint center = { (center_x * src.w * glm::abs(scale_x)),(center_y * src.h * glm::abs(scale_y)) }; // todo add support for x vs y scaling
			layer_obj_ref->centers[index] = (center);
			layer_obj_ref->scale_xs[index] = (scale_x);
			layer_obj_ref->scale_ys[index] = (scale_y);
			layer_obj_ref->center_xs[index] = (center_x);
			layer_obj_ref->center_ys[index] = (center_y);
			// save flip
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (scale_x < 0)
			{
				flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | flip);
			}
			if (scale_y < 0)
			{
				flip = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | flip);
			}
			layer_obj_ref->flips[index] = (flip);
			layer_obj_ref->rot_degrees[index] = (rot_degrees);
			// save empty des
			src.w *= scale_x;
			src.h *= scale_y;
			src.x = x;
			src.y = y;
			layer_obj_ref->dess[index] = (src);
			// save alive
			layer_obj_ref->alive[index] = (true);

			layer_obj_ref->sorting_order[index] = (sorting_order);

		}
	
		sprite_id++;
		index_to_data[sprite_id].index = index;
		index_to_data[sprite_id].layer = layer;
		index_to_data[sprite_id].UI = is_UI;
		return sprite_id;
	}
	static inline void SetSortOnLayer(int Layer,bool UI, bool do_sort) 
	{
		if (UI) 
		{
			UI_layers[Layer].do_sort = do_sort;
		}
		else
		{
			world_layers[Layer].do_sort = do_sort;
		}
	}
	static inline void FreeSpriteSlot(int id) 
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI) 
		{
			layer = &UI_layers[tmp.layer];
		}
		else 
		{
			layer = &world_layers[tmp.layer];
		}

		layer->alive[tmp.index] = false;
		layer->freed_index.push(tmp.index);
	}
	// need systems for handling ui vs world
	static inline b2Vec2 GetPositionSprite(int id) 
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}

		SDL_FRect tmp_frect = layer->dess[tmp.index];
		return { tmp_frect.x,tmp_frect.y };
		
	}
	static inline void SetPositionSprite(int id, b2Vec2 pos)
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}

		layer->dess[tmp.index].x = pos.x;
		layer->dess[tmp.index].y = pos.y;
		

	}
	static inline float GetRotationSprite(int id)
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}

		return layer->rot_degrees[tmp.index];
	}
	static inline void SetRotationSprite(int id, float rotation)
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}

		layer->rot_degrees[tmp.index] = rotation;
	}
	static inline void UpdateStoredSprites() 
	{
		std::vector<LayerDrawRequest> draw_requests;
		

		for(auto i = world_layers.begin(); i != world_layers.end(); i++)
		{
			LayerObject& layer = i->second;
		
			float x_scale_world = 1 / ZoomFactor;
			float y_scale_world = 1 / ZoomFactor;
			int n = 0;
			for (int j = 0; j < layer.alive.size(); j++) 
			{
				SDL_FRect tmp = layer.dess[j];
				SDL_FPoint tmp_point = layer.centers[j];
				tmp.x = (((tmp.x - current_pos.x) * 100) + width_r * x_scale_world * 0.5f - tmp_point.x);
				tmp.y = (((tmp.y - current_pos.y) * 100) + height_r * y_scale_world * 0.5f - tmp_point.y);
				if (layer.alive[j] && gameEngineUtils::isOverlapping(tmp.w, tmp.h, tmp.x, tmp.y, width_r * x_scale_world, height_r * y_scale_world, 0, 0, false)) 
				{
					
					draw_requests.emplace_back(layer.textures[j], layer.srcs[j], tmp, layer.centers[j], layer.colors[j], layer.rot_degrees[j], layer.flips[j], layer.sorting_order[j]);
					n++;
				}
			}
			if (layer.do_sort) 
			{
				std::stable_sort(draw_requests.end() - n, draw_requests.end());
			}
			
		}

		for (auto i = UI_layers.begin(); i != UI_layers.end(); i++)
		{
			LayerObject& layer = i->second;

			float x_scale_world = 1 ;
			float y_scale_world = 1 ;
			int n = 0;
			for (int j = 0; j < layer.alive.size(); j++)
			{
				SDL_FRect tmp = layer.dess[j];
				SDL_FPoint tmp_point = layer.centers[j];
				tmp.x = ((tmp.x * 100) + width_r * x_scale_world * 0.5f - tmp_point.x);
				tmp.y = ((tmp.y * 100) + height_r * y_scale_world * 0.5f - tmp_point.y);
				if (layer.alive[j] && gameEngineUtils::isOverlapping(tmp.w, tmp.h, tmp.x, tmp.y, width_r * x_scale_world, height_r * y_scale_world, 0, 0, false))
				{
					// do draw request
					if (layer.alive[j] && gameEngineUtils::isOverlapping(tmp.w, tmp.h, tmp.x, tmp.y, width_r * x_scale_world, height_r * y_scale_world, 0, 0, false))
					{
						draw_requests.emplace_back(layer.textures[j], layer.srcs[j], tmp, layer.centers[j], layer.colors[j], layer.rot_degrees[j], layer.flips[j], layer.sorting_order[j]);
						n++;
					}
				}
			}
			if (layer.do_sort)
			{
				std::stable_sort(draw_requests.end() - n, draw_requests.end());
			}
		}
	
		for (int i = 0; i < draw_requests.size(); i++) 
		{
			SDL_Color col = draw_requests[i].color;
			SDL_SetTextureColorMod(draw_requests[i].text, col.r, col.g, col.b);
			SDL_SetTextureAlphaMod(draw_requests[i].text, col.a);
			Helper::SDL_RenderCopyEx(0, "", renderer, draw_requests[i].text, &draw_requests[i].src, &draw_requests[i].des, draw_requests[i].rotation, &draw_requests[i].center, draw_requests[i].flip);
			SDL_SetTextureColorMod(draw_requests[i].text, 255, 255, 255);
			SDL_SetTextureAlphaMod(draw_requests[i].text, 255);
		}
	
	}

	static inline b2Vec2 GetScales(int id) 
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}

		
		return { layer->scale_xs[tmp.index],layer->scale_ys[tmp.index] };
	}

	static inline b2Vec2 GetCenters(int id)
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}


		return { layer->center_xs[tmp.index],layer->center_ys[tmp.index] };
	}

	static inline void SetScales(int id, float x_s, float y_s) 
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}
		layer->scale_xs[tmp.index] = x_s;
		layer->scale_ys[tmp.index] = y_s;

		int index = tmp.index;

		SDL_FPoint center = { (layer->center_xs[tmp.index] * layer->srcs[tmp.index].w * glm::abs(layer->scale_xs[tmp.index])),(layer->center_ys[tmp.index] * layer->srcs[tmp.index].h * glm::abs(layer->scale_ys[tmp.index])) }; // todo add support for x vs y scaling
		layer->centers[index] = (center);

		layer->dess[tmp.index].w = layer->srcs[tmp.index].w * x_s;
		layer->dess[tmp.index].h = layer->srcs[tmp.index].h * y_s;

		SDL_RendererFlip flip = SDL_FLIP_NONE;
		if (layer->scale_xs[tmp.index] < 0)
		{
			flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | flip);
		}
		if (layer->scale_ys[tmp.index] < 0)
		{
			flip = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | flip);
		}
		layer->flips[index] = (flip);

	}

	static inline void SetCenters(int id, float x_s, float y_s)
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}
		layer->center_xs[tmp.index] = x_s;
		layer->center_ys[tmp.index] = y_s;

		int index = tmp.index;

		SDL_FPoint center = { (layer->center_xs[tmp.index] * layer->srcs[tmp.index].w * glm::abs(layer->scale_xs[tmp.index])),(layer->center_ys[tmp.index] * layer->srcs[tmp.index].h * glm::abs(layer->scale_ys[tmp.index])) }; // todo add support for x vs y scaling
		layer->centers[index] = (center);

		
		

	}

	static inline SDL_Color GetColor(int id) 
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}
		return layer->colors[tmp.index];
	}

	static inline void SetColor(int id, int r, int g, int b, int a) 
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}

		SDL_Color col = { r,g,b,a };

		layer->colors[tmp.index] = col;
	}

	static inline SDL_Texture* GetTextTexture(std::string content, std::string font, int font_size) 
	{
		SDL_Color col = { 255,255,255,255 };
		SDL_Surface* mySurf = TTF_RenderText_Solid(TextDB::GetFont(font,font_size), content.c_str(), col);
		SDL_Texture* myText = SDL_CreateTextureFromSurface(me->renderer, mySurf);
		return myText;
	}

	static inline void UpdateTexture(int id, SDL_Texture* text) 
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}

		int index = tmp.index;
		
		// get image ptr
		layer->textures[index] = (text);
		// save src 
		SDL_FRect src = { 0,0,0,0 };
		Helper::SDL_QueryTexture(text, &src.w, &src.h);
		layer->srcs[index] = (src);
		// save fpoint
		SDL_FPoint center = { (layer->center_xs[tmp.index] * layer->srcs[tmp.index].w * glm::abs(layer->scale_xs[tmp.index])),(layer->center_ys[tmp.index] * layer->srcs[tmp.index].h * glm::abs(layer->scale_ys[tmp.index])) }; // todo add support for x vs y scaling
		layer->centers[index] = (center);


		// save empty des
		src.w *= layer->scale_xs[tmp.index];
		src.h *= layer->scale_ys[tmp.index];
		
		layer->dess[index].h = (src).h;
		layer->dess[index].w = (src).w;
		// save alive
		
	}

	static inline void ChangeLayerUI(int id, int new_layer, bool new_is_ui) 
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}
		SDL_FRect des = layer->dess[tmp.index];
		SDL_Color col = layer->colors[tmp.index];
		int new_ID = ReserveSpriteSlot(layer->textures[tmp.index], des.x, des.y, layer->scale_xs[tmp.index], layer->scale_ys[tmp.index], layer->rot_degrees[tmp.index], layer->center_xs[tmp.index], layer->center_ys[tmp.index], col.r, col.g, col.b, col.a, layer->sorting_order[tmp.index], new_layer, new_is_ui);
		FreeSpriteSlot(id);
		index_to_data[id].index = index_to_data[new_ID].index;
		index_to_data[id].layer = index_to_data[new_ID].layer;
		index_to_data[id].UI = index_to_data[new_ID].UI;
	}

	static inline int GetLayer(int id) 
	{
		SpriteData tmp = index_to_data[id];
		return tmp.layer;
	}

	static inline int GetIsUI(int id)
	{
		SpriteData tmp = index_to_data[id];
		return tmp.UI;
	}

	static inline int GetSortingOrder(int id) 
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}
		return layer->sorting_order[tmp.index];
	}

	static inline void SetSortingOrder(int id, int newSortingOrder)
	{
		SpriteData tmp = index_to_data[id];
		LayerObject* layer;
		if (tmp.UI)
		{
			layer = &UI_layers[tmp.layer];
		}
		else
		{
			layer = &world_layers[tmp.layer];
		}
		layer->sorting_order[tmp.index] = newSortingOrder;
	}


};

