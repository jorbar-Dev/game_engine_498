#pragma once
#ifndef HITRESULT_H
#define HITRESULT_H



#include "box2d/box2d.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
class Actor_C;
class HitResult498
{
public:
	Actor_C* other;
	b2Vec2 point;
	b2Vec2 normal;
	bool is_trigger;


};

#endif // !1
