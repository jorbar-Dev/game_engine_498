#pragma once
#include "box2d/box2d.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
class Actor_C;
class collision
{
public:
	Actor_C* other;
	b2Vec2 point;
	b2Vec2 relative_velocity;
	b2Vec2 normal;

	static inline void LuaToCollision(lua_State* lua_s) 
	{
		luabridge::getGlobalNamespace(lua_s)
			.beginClass<collision>("collision")
			.addProperty("normal", &collision::normal)
			.addProperty("relative_velocity", &collision::relative_velocity)
			.addProperty("point", &collision::point)
			.addProperty("other", &collision::other)
			.endClass();
	}
};

