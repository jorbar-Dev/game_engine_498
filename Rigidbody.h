#pragma once
#include <iostream>
#include <string>
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "unordered_map"
#include "box2d/box2d.h"
#include "HitResult498.h"
#include "RayCastCallBack498.h"
class Actor_C;
class ContactListener498;
class Rigidbody
{
public:
	static inline b2ContactListener* contact_listener;
	static inline b2World* physicsWorld;
	static inline lua_State* lua_s;

	static void LuaToRigidbody(lua_State* lua_state, std::unordered_map<std::string,std::string>* valid_components) 
	{
		lua_s = lua_state;
		luabridge::getGlobalNamespace(lua_state)
			.beginClass<Rigidbody>("Rigidbody")
			.addFunction("OnStart", &Rigidbody::OnStart)
			.addFunction("OnDestroy", &Rigidbody::OnDestroy)
			.addFunction("GetPosition", &Rigidbody::GetPosition)
			.addFunction("GetRotation", &Rigidbody::GetRotation)
			.addProperty("actor", &Rigidbody::actor)
			.addProperty("enabled", &Rigidbody::enabled)
			.addProperty("key", &Rigidbody::key)
			.addProperty("type", &Rigidbody::type)
			.addProperty("x",&Rigidbody::x)
			.addProperty("y", &Rigidbody::y)
			.addProperty("body_type", &Rigidbody::bodytype)
			.addProperty("trigger_type", &Rigidbody::trigger_type)
			.addProperty("precise", &Rigidbody::precise)
			.addProperty("gravity_scale", &Rigidbody::gravity_scale)
			.addProperty("density", &Rigidbody::density)
			.addProperty("angular_friction", &Rigidbody::angular_friction)
			.addProperty("rotation", &Rigidbody::rotation)
			.addProperty("has_collider", &Rigidbody::has_collider)
			.addProperty("has_trigger", &Rigidbody::has_trigger)
			.addFunction("AddForce", &Rigidbody::AddForce)
			.addFunction("SetVelocity", &Rigidbody::SetVelocity)
			.addFunction("SetPosition", &Rigidbody::SetPosition)
			.addFunction("SetRotation", &Rigidbody::SetRotation)
			.addFunction("SetAngularVelocity", &Rigidbody::SetAngularVelocity)
			.addFunction("SetGravityScale", &Rigidbody::SetGravityScale)
			.addFunction("SetUpDirection", &Rigidbody::SetUpDirection)
			.addFunction("SetRightDirection", &Rigidbody::SetRightDirection)
			.addFunction("GetVelocity", &Rigidbody::GetVelocity)
			.addFunction("GetAngularVelocity", &Rigidbody::GetAnglularVelocity)
			.addFunction("GetGravityScale", &Rigidbody::GetGravityScale)
			.addFunction("GetUpDirection", &Rigidbody::GetUpDirection)
			.addFunction("GetRightDirection", &Rigidbody::GetRightDirection)
			.addProperty("collider_type",&Rigidbody::collider_type)
			.addProperty("width", &Rigidbody::width)
			.addProperty("height", &Rigidbody::height)
			.addProperty("radius", &Rigidbody::radius)
			.addProperty("trigger_width", &Rigidbody::trigger_width)
			.addProperty("trigger_height", &Rigidbody::trigger_height)
			.addProperty("trigger_radius", &Rigidbody::trigger_radius)
			.addProperty("friction", &Rigidbody::friction)
			.addProperty("bounciness", &Rigidbody::bounciness)
			.endClass();

		(*valid_components)["Rigidbody"] = "Rigidbody";

		luabridge::getGlobalNamespace(lua_s)
			.beginClass<HitResult498>("HitResult")
			.addProperty("actor", &HitResult498::other)
			.addProperty("point", &HitResult498::point)
			.addProperty("is_trigger", &HitResult498::is_trigger)
			.addProperty("normal", &HitResult498::normal)
			.endClass();

		luabridge::getGlobalNamespace(lua_s)
			.beginNamespace("Physics")
			.addFunction("Raycast", &Rigidbody::Raycast)
			.addFunction("RaycastAll", &Rigidbody::RaycastALL)
			.endNamespace();
	}

	// General component values
	bool enabled = true;
	Actor_C* actor = NULL;
	std::string key = "DefaultCPPKey";
	std::string type = "Rigidbody";
	bool has_ran_onstart = false;

	// Rigidbody Component Values
	float x = 0.0f;
	float y = 0.0f;
	std::string bodytype = "dynamic";
	bool precise = true;
	float gravity_scale = 1.0f;
	float density = 1.0f;
	float angular_friction = 0.3f;
	float rotation = 0.0f;
	bool has_collider = true;
	bool has_trigger = true;

	// test suite 3
	std::string collider_type = "box";
	float width = 1.0f;
	float height = 1.0f;
	float radius = 0.5f;
	float friction = 0.3f;
	float bounciness = 0.3f;

	// trigger suite
	std::string trigger_type = "box";
	float trigger_width = 1.0f;
	float trigger_height = 1.0f;
	float trigger_radius = 0.5f;


	//private
	b2Body* body;
	void OnStart() 
	{
		rotation = rotation * (b2_pi / 180.0f);
		if (Rigidbody::physicsWorld == NULL) 
		{
			b2Vec2 gravity = { 0.0f,9.8f };
			Rigidbody::physicsWorld = new b2World(gravity);
			Rigidbody::physicsWorld->SetContactListener(contact_listener);
		}
		b2BodyDef body_def;
		if (bodytype == "dynamic") 
		{
			body_def.type = b2_dynamicBody;
		}
		else if (bodytype == "kinematic")
		{
			body_def.type = b2_kinematicBody;
		}
		else if (bodytype == "static")
		{
			body_def.type = b2_staticBody;
		}

		body_def.gravityScale = gravity_scale;
		body_def.position = b2Vec2(x, y);
		body_def.bullet = precise;
		body_def.angle = rotation;
		body_def.angularDamping = angular_friction;

		body = physicsWorld->CreateBody(&body_def);

		// set up details
		b2PolygonShape my_shape;
		b2CircleShape my_circle;
		b2FixtureDef fixture;
		

		// General Properties
		fixture.density = density;
		fixture.friction = friction;
		fixture.restitution = bounciness;

		fixture.userData.pointer = reinterpret_cast<uintptr_t>(actor);

		
		if (!has_collider && !has_trigger) 
		{
			fixture.isSensor = true;
			my_shape.SetAsBox( width*0.5f,  height*0.5f);
			fixture.filter.categoryBits = (uint16)(0);
			fixture.filter.maskBits = (uint16)(0);
			fixture.shape = &my_shape;
			body->CreateFixture(&fixture);
		}
		if(has_collider)
		{
			if (collider_type == "box")
			{
				my_shape.SetAsBox(width * 0.5f, height * 0.5f);
				fixture.shape = &my_shape;
			}
			else
			{
				my_circle.m_radius = radius;
				fixture.shape = &my_circle;
			}
			fixture.filter.categoryBits = (uint16)(1);
			fixture.filter.maskBits = (uint16)(1);
			fixture.isSensor = false;
			body->CreateFixture(&fixture);
		}
		if(has_trigger)
		{
			if (trigger_type == "box")
			{
				my_shape.SetAsBox(trigger_width * 0.5f, trigger_height * 0.5f);
				fixture.shape = &my_shape;
			}
			else
			{
				my_circle.m_radius = trigger_radius;
				fixture.shape = &my_circle;
			}
			fixture.filter.categoryBits = (uint16)(2);
			fixture.filter.maskBits = (uint16)(2);
			fixture.isSensor = true;
			body->CreateFixture(&fixture);
		}

	}

	void OnDestroy() 
	{
		if (body == NULL)
		{
			return;
		}
		physicsWorld->DestroyBody(body);
	}

	b2Vec2 GetPosition()
	{
		if (body == NULL)
		{
			return {x,y};
		}
		
		return body->GetPosition();
	}

	float GetRotation()
	{
		if (body == NULL)
		{
			return rotation;
		}
		return body->GetAngle() * (180.0f / b2_pi);
	}

	void AddForce(b2Vec2 Vector2) 
	{
		if (body == NULL)
		{
			return;
		}
		body->ApplyForceToCenter(Vector2, true);
	}

	void SetVelocity(b2Vec2 Vector2) 
	{
		if (body == NULL)
		{
			return;
		}
		body->SetLinearVelocity(Vector2);
	}

	void SetPosition(b2Vec2 Vector2) 
	{
		if (body == NULL)
		{
			x = Vector2.x;
			y = Vector2.y;
			return;
		}
		float rotation_tmp = body->GetAngle();
		body->SetTransform(Vector2, rotation_tmp);
	}

	void SetRotation(float degrees) 
	{
		if (body == NULL) 
		{
			rotation = degrees;
			return;
		}
		float rotation_tmp = degrees * (b2_pi / 180.0f);
		body->SetTransform(body->GetPosition(), rotation_tmp);
	}

	void SetAngularVelocity(float degrees) 
	{
		if (body == NULL)
		{
			return;
		}
		float rotation_tmp = degrees * (b2_pi / 180.0f);
		body->SetAngularVelocity(rotation_tmp);
	}

	void SetGravityScale(float grav_scale) 
	{
		if (body == NULL)
		{
			gravity_scale = grav_scale;
			return;
		}
		body->SetGravityScale(grav_scale);
	}

	void SetUpDirection(b2Vec2 direction) 
	{
		if (body == NULL)
		{
			return;
		}
		direction.Normalize();
		float rotation = glm::atan(direction.x, -direction.y);
		body->SetTransform(body->GetPosition(), rotation);

	}

	void SetRightDirection(b2Vec2 direction) 
	{
		if (body == NULL)
		{
			return;
		}
		direction.Normalize();
		float rotation = glm::atan(direction.x, -direction.y) - b2_pi / 2.0f;
		body->SetTransform(body->GetPosition(), rotation);
	}

	b2Vec2 GetVelocity() 
	{
		if (body == NULL)
		{
			return{0,0};
		}
		return body->GetLinearVelocity();
	}

	float GetAnglularVelocity() 
	{
		if (body == NULL)
		{
			return 0;
		}
		return body->GetAngularVelocity() * (180.0f / b2_pi);
	}

	float GetGravityScale() 
	{
		if (body == NULL)
		{
			return gravity_scale;
		}
		return body->GetGravityScale();
	}

	b2Vec2 GetUpDirection() 
	{
		if (body == NULL)
		{
			return {0,1};
		}
		float angle = body->GetAngle() - b2_pi/2.0f;
		return b2Vec2(glm::cos(angle), glm::sin(angle)) ;
	}

	b2Vec2 GetRightDirection() 
	{
		if (body == NULL)
		{
			return { 1,0 };
		}
		float angle = body->GetAngle();
		return b2Vec2(glm::cos(angle), glm::sin(angle));
	}

	static luabridge::LuaRef Raycast(b2Vec2 pos, b2Vec2 dir, float dist) 
	{
		if (physicsWorld == NULL || dist <= 0)
		{
			return luabridge::LuaRef(lua_s); // change to NIL
		}
		RayCastCallBack498 callback;
		b2Vec2 end_pt = pos;
		b2Vec2 tmp_pt = dir;
		tmp_pt.Normalize();
		tmp_pt *= dist;
		end_pt += tmp_pt;
		physicsWorld->RayCast(&callback, pos, end_pt);
		if (callback.myCallBacks.size() > 0) 
		{
			HitResult498 myHit;
			myHit.other = reinterpret_cast<Actor_C*> (callback.myCallBacks.front().callback->GetUserData().pointer);
			myHit.is_trigger = callback.myCallBacks.front().callback->GetFilterData().maskBits == (uint16)2;
			myHit.normal = callback.myCallBacks.front().normal;
			myHit.point = callback.myCallBacks.front().point;

			return luabridge::LuaRef(lua_s, myHit);
		}

		return luabridge::LuaRef(lua_s);
	}
	static luabridge::LuaRef RaycastALL(b2Vec2 pos, b2Vec2 dir, float dist)
	{
		if (physicsWorld == NULL || dist <= 0)
		{
			return luabridge::LuaRef(lua_s); // change to NIL
		}
		RayCastCallBack498 callback;
		callback.return_all = true;
		b2Vec2 end_pt = pos;
		b2Vec2 tmp_pt = dir;
		tmp_pt.Normalize();
		tmp_pt *= dist;
		end_pt += tmp_pt;
		physicsWorld->RayCast(&callback, pos, end_pt);
		luabridge::LuaRef table = luabridge::LuaRef(luabridge::newTable(lua_s));
		if (callback.myCallBacks.size() > 0)
		{
			std::stable_sort(callback.myCallBacks.begin(), callback.myCallBacks.end(), [](const callBack& first, const callBack& second)
				{
					return first.fraction < second.fraction;
				});
			for (int i = 0; i < callback.myCallBacks.size(); i++) 
			{
				HitResult498 myHit;
				myHit.other = reinterpret_cast<Actor_C*>(callback.myCallBacks[i].callback->GetUserData().pointer);
				myHit.is_trigger = callback.myCallBacks[i].callback->GetFilterData().maskBits == (uint16)2;
				myHit.normal = callback.myCallBacks[i].normal;
				myHit.point = callback.myCallBacks[i].point;
				
				table[i + 1] = luabridge::LuaRef(lua_s, myHit);
			}

			return table;
		}

		return table;
	}





};

