#pragma once
#include <iostream>
#include <string>
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "unordered_map"
#include "box2d/box2d.h"
#include "RendererHandler.h"
class Actor_C;
class SpriteRenderer_C
{
public:
	static inline lua_State* lua_s;

	static void LuaToSpriteRenderer_C(lua_State* lua_state, std::unordered_map<std::string, std::string>* valid_components)
	{
		lua_s = lua_state;
		luabridge::getGlobalNamespace(lua_state)
			.beginClass<SpriteRenderer_C>("SpriteRenderer_C")
			.addFunction("OnStart", &SpriteRenderer_C::OnStart)
			.addFunction("OnDestroy", &SpriteRenderer_C::OnDestroy)
			.addProperty("actor", &SpriteRenderer_C::actor)
			.addProperty("enabled", &SpriteRenderer_C::enabled)
			.addProperty("key", &SpriteRenderer_C::key)
			.addProperty("type", &SpriteRenderer_C::type)
			.addProperty("x", &SpriteRenderer_C::x)
			.addProperty("y", &SpriteRenderer_C::y)
			.addProperty("rot_degrees", &SpriteRenderer_C::rot_degrees)
			.addProperty("r", &SpriteRenderer_C::r)
			.addProperty("g", &SpriteRenderer_C::g)
			.addProperty("b", &SpriteRenderer_C::b)
			.addProperty("a", &SpriteRenderer_C::a)
			.addProperty("sorting_order", &SpriteRenderer_C::sorting_order)
			.addProperty("layer", &SpriteRenderer_C::layer)
			.addProperty("is_UI", &SpriteRenderer_C::is_UI)
			.addProperty("sprite", &SpriteRenderer_C::sprite)
			.addProperty("text_content", &SpriteRenderer_C::text_content)
			.addProperty("font", &SpriteRenderer_C::font)
			.addProperty("font_size", &SpriteRenderer_C::font_size)
			.addProperty("is_text_renderer", &SpriteRenderer_C::is_text_renderer)
			.addProperty("center_X", &SpriteRenderer_C::center_x)
			.addProperty("center_Y", &SpriteRenderer_C::center_y)
			.addFunction("GetPosition", &SpriteRenderer_C::GetPosition)
			.addFunction("GetRotation", &SpriteRenderer_C::GetRotation)
			.addFunction("SetPosition", &SpriteRenderer_C::SetPosition)
			.addFunction("SetRotation", &SpriteRenderer_C::SetRotation)
			.addFunction("SetScales", &SpriteRenderer_C::SetScales)
			.addFunction("GetScales", &SpriteRenderer_C::GetScales)
			.addFunction("SetCenters", &SpriteRenderer_C::SetCenters)
			.addFunction("GetCenters", &SpriteRenderer_C::GetCenters)
			.addFunction("SetColor", &SpriteRenderer_C::SetColor)
			.addFunction("GetR", &SpriteRenderer_C::GetR)
			.addFunction("GetG", &SpriteRenderer_C::GetG)
			.addFunction("GetB", &SpriteRenderer_C::GetB)
			.addFunction("GetA", &SpriteRenderer_C::GetA)
			.addFunction("SetSprite", &SpriteRenderer_C::SetSprite)
			.addFunction("SetText", &SpriteRenderer_C::SetText)
			.addFunction("ChangeLayerAndUI", &SpriteRenderer_C::ChangeLayerAndUI)
			.addFunction("GetLayer", &SpriteRenderer_C::GetLayer)
			.addFunction("GetIsUI", &SpriteRenderer_C::GetUI)
			.addFunction("SetSortingOrder", &SpriteRenderer_C::SetSortingOrder)
			.addFunction("GetSortingOrder", &SpriteRenderer_C::GetSortingOrder)
			.endClass();

		/*
			std::string text_content = "";
			std::string font_size = "";
			int font_size = 16;
			bool is_text_renderer = false;
		*/


		luabridge::getGlobalNamespace(lua_state)
			.beginNamespace("RenderLayerControl")
			.addFunction("SetLayerDoSort", &RendererHandler::SetSortOnLayer)
			.endNamespace();

		(*valid_components)["SpriteRenderer_C"] = "SpriteRenderer_C";

	}

	// General component values
	bool enabled = true;
	Actor_C* actor = NULL;
	std::string key = "DefaultCPPKey";
	std::string type = "SpriteRenderer_C";
	bool has_ran_onstart = false;
	bool Alive = false;
	// ID
	int ID = -1;

	// Sprite start properties
		// scale
		// x
		// y
		// rotation rot_degrees
		// r
		// g
		// b
		// a
		// sorting order
		// center x
		// center y
		// sprite string

	float x = 0;
	float y = 0;
	float scale_x = 1;
	float scale_y = 1;
	float rot_degrees = 0.0f;
	int r = 255;
	int g = 255;
	int b = 255;
	int a = 255;
	int sorting_order = 0;
	float center_x = .5f;
	float center_y = .5f;
	std::string sprite = "";
	std::string text_content = "";
	std::string font = "";
	int font_size = 16;
	bool is_text_renderer = false;
	int layer = 0;
	bool is_UI = false;

	void OnStart() 
	{

		if (r > 255 || r < 0) 
		{
			r = 255;
			std::cout << "Hello, engine developer here, r must be between 0 and 255" << std::endl;
		}
		if (g > 255 || g < 0)
		{
			g = 255;
			std::cout << "Hello, engine developer here, g must be between 0 and 255" << std::endl;
		}
		if (b > 255 || b < 0)
		{
			b = 255;
			std::cout << "Hello, engine developer here, b must be between 0 and 255" << std::endl;
		}
		if (a > 255 || a < 0)
		{
			a = 255;
			std::cout << "Hello, engine developer here, a must be between 0 and 255" << std::endl;
		}
		SDL_Texture* text;
		if (is_text_renderer) 
		{
			text = RendererHandler::GetTextTexture(text_content, font, font_size);
		}
		else 
		{
			text = ImageDB::LoadImage(sprite);
		}
		ID = RendererHandler::ReserveSpriteSlot(text,x,y,scale_x,scale_y,rot_degrees,center_x,center_y,r,g,b,a,sorting_order,layer,is_UI);

		Alive = true;
		has_ran_onstart = true;
	}

	void OnDestroy() 
	{
		if (!Alive) 
		{
			return;
		}
		Alive = false;
		RendererHandler::FreeSpriteSlot(ID);
	}

	b2Vec2 GetPosition() { 
		return RendererHandler::GetPositionSprite(ID); 
	}

	float GetRotation() { 
		if (!Alive)
		{
			return 0;
		}
		
		return RendererHandler::GetRotationSprite(ID); 
	}

	void SetPosition(b2Vec2 pos) 
	{ 
		x = pos.x;
		y = pos.y;
		if (!Alive)
		{
			return;
		}
		RendererHandler::SetPositionSprite(ID, pos); 
		//std::cout << pos.x << " is my x and " << pos.y << " is my y" << std::endl;
	}

	void SetRotation(float rotation_in) { 
		
		rot_degrees = rotation_in;
		if (!Alive)
		{
			return;
		}
		RendererHandler::SetRotationSprite(ID, rotation_in); 
	}

	b2Vec2 GetScales() { 
		if (!Alive)
		{
			return b2Vec2(scale_x,scale_y);
		}
		return RendererHandler::GetScales(ID); 
	}

	void SetScales( float x_s, float y_s) 
	{
		scale_x = x_s;
		scale_y = y_s;
		if (!Alive)
		{
			return;
		}
		RendererHandler::SetScales(ID, x_s, y_s);
	}

	b2Vec2 GetCenters() {
		if (!Alive)
		{
			return b2Vec2(center_x, center_y);
		}
		return RendererHandler::GetCenters(ID);
	}

	void SetCenters(float x_s, float y_s)
	{
		center_x = x_s;
		center_y = y_s;
		if (!Alive)
		{
			return;
		}
		RendererHandler::SetCenters(ID, x_s, y_s);
	}

	int GetR() 
	{
		if (!Alive)
		{
			return r;
		}
		RendererHandler::GetColor(ID).r;
	}

	int GetG() 
	{
		if (!Alive)
		{
			return g;
		}
		RendererHandler::GetColor(ID).g;
	}

	int GetB() 
	{
		if (!Alive)
		{
			return b;
		}
		RendererHandler::GetColor(ID).b;
	}

	int GetA() 
	{
		if (!Alive)
		{
			return a;
		}
		RendererHandler::GetColor(ID).a;
	}

	void SetColor(int R, int G, int B, int A) 
	{
		r = R;
		g = G;
		b = B;
		a = A;

		if (r > 255 || r < 0)
		{
			r = 255;
			std::cout << "Hello, engine developer here, r must be between 0 and 255" << std::endl;
		}
		if (g > 255 || g < 0)
		{
			g = 255;
			std::cout << "Hello, engine developer here, g must be between 0 and 255" << std::endl;
		}
		if (b > 255 || b < 0)
		{
			b = 255;
			std::cout << "Hello, engine developer here, b must be between 0 and 255" << std::endl;
		}
		if (a > 255 || a < 0)
		{
			a = 255;
			std::cout << "Hello, engine developer here, a must be between 0 and 255" << std::endl;
		}
		if (!Alive)
		{
			return;
		}
		RendererHandler::SetColor(ID, r, g, b, a);
	}

	void SetSprite(std::string content) 
	{
		sprite = content;
		if (!Alive)
		{
			return;
		}
		RendererHandler::UpdateTexture(ID, ImageDB::LoadImage(sprite));
	}

	void SetText(std::string content, std::string new_font, int new_font_size) 
	{
		text_content = content;
		if (!Alive)
		{
			return;
		}
		RendererHandler::UpdateTexture(ID, RendererHandler::GetTextTexture(content,new_font,new_font_size));
	}

	void ChangeLayerAndUI(int Layer, bool Is_ui) 
	{
		layer = Layer;
		Is_ui = Is_ui;
		if (!Alive)
		{
			return;
		}
		RendererHandler::ChangeLayerUI(ID, Layer, Is_ui);
	}

	int GetLayer() 
	{
		if (!Alive)
		{
			return layer;
		}
		return RendererHandler::GetLayer(ID);
	}

	bool GetUI() 
	{
		if (!Alive)
		{
			return is_UI;
		}
		return RendererHandler::GetIsUI(ID);
	}

	int GetSortingOrder() 
	{
		if (!Alive)
		{
			return sorting_order;
		}
		return RendererHandler::GetSortingOrder(ID);
	}

	void SetSortingOrder(int order) 
	{
		if (!Alive)
		{
			sorting_order = order;
			return;
		}
		RendererHandler::SetSortingOrder(ID, order);
	}
};

