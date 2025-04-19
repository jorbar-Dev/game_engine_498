#pragma once
#include <iostream>
#include <string>
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "unordered_map"
#include "box2d/box2d.h"
#include <vector>
#include "ImageDB.h"
#include "Helper.h"
#include "RendererHandler.h"
#include "glm/glm.hpp"
class Actor_C;
class ParticleSystem498
{
public:

	static inline lua_State* lua_s;

	Actor_C* actor;
	bool enabled = true;
	std::string key;
	std::string type = "ParticleSystem";
	std::string DefaultParticleName = "DefaultParticle";
	std::string ParticleName = "";
	RandomEngine myRandomEngineAngle;
	RandomEngine myRandomEngineRadius;
	RandomEngine myRandomEngineRot;
	RandomEngine myRandomEngineStartScale;
	RandomEngine myRandomEngineSpeed;
	RandomEngine myRandomEngineRotSpeed;

	// default values

	float emit_angle_min = 0.0f;
	float emit_angle_max = 360.0f;

	float emit_radius_min = 0.0f;
	float emit_radius_max = 0.5f;

	float emit_rotation_min = 0.0f;
	float emit_rotation_max = 0.0f;

	float start_scaling_min = 1.0f;
	float start_scaling_max = 1.0f;

	float start_speed_min = 0.0f;
	float start_speed_max = 0.0f;

	float speed_rot_min = 0.0f;
	float speed_rot_max = 0.0f;

	std::vector<int> free_slots;

	std::vector<bool> Alive;
	std::vector<float> POS_X;
	std::vector<float> POS_Y;
	std::vector<float> SPEED_X;
	std::vector<float> SPEED_Y;
	std::vector<float> ROT_;
	std::vector<float> SPEED_ROT_;
	std::vector<float> SCALE_;
	std::vector<float> SCALE_S;
	std::vector<float> COL_R;
	std::vector<float> COL_G;
	std::vector<float> COL_B;

	std::vector<float> COL_R_S;
	std::vector<float> COL_G_S;
	std::vector<float> COL_B_S;


	std::vector<float> COL_A_S;

	std::vector<float> COL_A;

	std::vector<int> framesAlive;
	int sorting_order = 9999;

	float x = 0.0f;
	float y = 0.0f;

	int frames_between_bursts = 1;
	int burst_quantity = 1;

	int local_frame_num = 0;

	int StartColorR = 255;
	int StartColorG = 255;
	int StartColorB = 255;
	int StartColorA = 255;

	int duration_frames = 300;

	float gravity_scale_x = 0.0f;
	float gravity_scale_y = 0.0f;
	float drag_factor = 1.0f;
	float angular_drag_factor = 1.0f;
	std::string end_scale = "";
	float end_scale_f = 0;
	std::string end_color_r = "";
	float end_color_r_f = 0;
	std::string end_color_g = "";
	float end_color_g_f = 0;
	std::string end_color_b = "";
	float end_color_b_f = 0;
	std::string end_color_a = "";
	float end_color_a_f = 0;
	bool Running = true;


	static void LuaToParticleSystem(lua_State* lua_state, std::unordered_map<std::string, std::string>* valid_components)
	{
		lua_s = lua_state;
		luabridge::getGlobalNamespace(lua_state)
			.beginClass<ParticleSystem498>("ParticleSystem")
			.addFunction("OnStart", &ParticleSystem498::OnStart)
			.addFunction("OnUpdate", &ParticleSystem498::OnUpdate)
			.addFunction("OnDestroy", &ParticleSystem498::OnDestroy)
			.addProperty("actor", &ParticleSystem498::actor)
			.addProperty("enabled", &ParticleSystem498::enabled)
			.addProperty("key", &ParticleSystem498::key)
			.addProperty("type", &ParticleSystem498::type)
			.addProperty("x", &ParticleSystem498::x)
			.addProperty("y", &ParticleSystem498::y)
			.addProperty("frames_between_bursts", &ParticleSystem498::frames_between_bursts)
			.addProperty("burst_quantity", &ParticleSystem498::burst_quantity)
			.addProperty("start_scale_min", &ParticleSystem498::start_scaling_min)
			.addProperty("start_scale_max", &ParticleSystem498::start_scaling_max)
			.addProperty("rotation_min", &ParticleSystem498::emit_rotation_min)
			.addProperty("rotation_max", &ParticleSystem498::emit_rotation_max)
			.addProperty("start_color_r", &ParticleSystem498::StartColorR)
			.addProperty("start_color_g", &ParticleSystem498::StartColorG)
			.addProperty("start_color_b", &ParticleSystem498::StartColorB)
			.addProperty("start_color_a", &ParticleSystem498::StartColorA)
			.addProperty("emit_radius_min", &ParticleSystem498::emit_radius_min)
			.addProperty("emit_radius_max", &ParticleSystem498::emit_radius_max)
			.addProperty("emit_angle_min", &ParticleSystem498::emit_angle_min)
			.addProperty("emit_angle_max", &ParticleSystem498::emit_angle_max)
			.addProperty("image", &ParticleSystem498::ParticleName)
			.addProperty("sorting_order", &ParticleSystem498::sorting_order)
			.addProperty("end_color_r", &ParticleSystem498::end_color_r)
			.addProperty("end_color_g", &ParticleSystem498::end_color_g)
			.addProperty("end_color_b", &ParticleSystem498::end_color_b)
			.addProperty("end_color_a", &ParticleSystem498::end_color_a)
			.addProperty("end_scale", &ParticleSystem498::end_scale)
			.addProperty("angular_drag_factor", &ParticleSystem498::angular_drag_factor)
			.addProperty("drag_factor", &ParticleSystem498::drag_factor)
			.addProperty("gravity_scale_x", &ParticleSystem498::gravity_scale_x)
			.addProperty("gravity_scale_y", &ParticleSystem498::gravity_scale_y)
			.addProperty("rotation_speed_min", &ParticleSystem498::speed_rot_min)
			.addProperty("rotation_speed_max", &ParticleSystem498::speed_rot_max)
			.addProperty("start_speed_min", &ParticleSystem498::start_speed_min)
			.addProperty("start_speed_max", &ParticleSystem498::start_speed_max)
			.addProperty("duration_frames", &ParticleSystem498::duration_frames)
			.addFunction("Burst",&ParticleSystem498::Burst)
			.addFunction("Play", &ParticleSystem498::Play)
			.addFunction("Stop", &ParticleSystem498::Stop)
			.endClass();

		(*valid_components)["ParticleSystem"] = "ParticleSystem";

	}

	void OnStart() 
	{

		// just because we know that the particles are going to generate alot




		ImageDB::CreateDefaultParticleTextureWithName(DefaultParticleName);
		
		myRandomEngineAngle.Configure(emit_angle_min, emit_angle_max, 298);
		myRandomEngineRadius.Configure(emit_radius_min, emit_radius_max, 404);

		myRandomEngineRot.Configure(emit_rotation_min, emit_rotation_max, 440);
		myRandomEngineStartScale.Configure(start_scaling_min, start_scaling_max, 494);

		myRandomEngineSpeed.Configure(start_speed_min, start_speed_max, 498);
		myRandomEngineRotSpeed.Configure(speed_rot_min, speed_rot_max, 305);

		if (end_scale != "") 
		{
			end_scale_f = atof(end_scale.c_str());
		}
		if (end_color_a != "") 
		{
			end_color_a_f = atof(end_color_a.c_str());
		}
		if (end_color_r != "")
		{
			end_color_r_f = atof(end_color_r.c_str());
		}
		if (end_color_g != "")
		{
			end_color_g_f = atof(end_color_g.c_str());
		}
		if (end_color_b != "")
		{
			end_color_b_f = atof(end_color_b.c_str());
		}
		if (duration_frames < 1) 
		{
			duration_frames = 1;
		}
		if (frames_between_bursts < 1)
		{
			frames_between_bursts = 1;
		}
		if (burst_quantity < 1)
		{
			burst_quantity = 1;
		}


		int num_to_reserve = burst_quantity * duration_frames; // small num

		Alive.reserve(num_to_reserve);
		POS_X.reserve(num_to_reserve);
		POS_Y.reserve(num_to_reserve);
		SPEED_X.reserve(num_to_reserve);
		SPEED_Y.reserve(num_to_reserve);
		ROT_.reserve(num_to_reserve);
		SPEED_ROT_.reserve(num_to_reserve);
		SCALE_.reserve(num_to_reserve);
		SCALE_S.reserve(num_to_reserve);
		COL_R.reserve(num_to_reserve);
		COL_G.reserve(num_to_reserve);
		COL_B.reserve(num_to_reserve);

		COL_R_S.reserve(num_to_reserve);
		COL_G_S.reserve(num_to_reserve);
		COL_B_S.reserve(num_to_reserve);


		COL_A_S.reserve(num_to_reserve);

		COL_A.reserve(num_to_reserve);

		framesAlive.reserve(num_to_reserve);

	}

	void OnUpdate() 
	{
		if (!enabled ) 
		{
			return;
		}
		if (Running) 
		{
			for (int i = 0; i < Alive.size(); i++)
			{
				if (!Alive[i])
				{
					continue;
				}
				framesAlive[i] += 1;
			
				if (framesAlive[i] == duration_frames) 
				{
					free_slots.push_back(i);
					Alive[i] = false;
			
				}
			}

			// do burst
			DoBurst();
		}
			for (int i = 0; i < Alive.size(); i++)
			{
				if (!Alive[i])
				{
					continue;
				}
				SPEED_X[i] += gravity_scale_x;
				SPEED_Y[i] += gravity_scale_y;

				SPEED_X[i] *= drag_factor;
				SPEED_Y[i] *= drag_factor;
				POS_X[i] += SPEED_X[i];
				POS_Y[i] += SPEED_Y[i];
			}
			for (int i = 0; i < Alive.size(); i++)
			{
				if (!Alive[i])
				{
					continue;
				}
				SPEED_ROT_[i] *= angular_drag_factor;
				ROT_[i] += SPEED_ROT_[i];
			}
			if (end_scale != "") 
			{
		
				for (int i = 0; i < Alive.size(); i++)
				{
					if (!Alive[i])
					{
						continue;
					}
					float life_progress = static_cast<float> (framesAlive[i]) / duration_frames;

					SCALE_[i] = glm::mix(SCALE_S[i], end_scale_f, life_progress);
				}
			}
			if (end_color_a != "")
			{

				for (int i = 0; i < Alive.size(); i++)
				{
					if (!Alive[i])
					{
						continue;
					}
					float life_progress = static_cast<float> (framesAlive[i]) / duration_frames;

					COL_A[i] = glm::mix(COL_A_S[i], end_color_a_f, life_progress);
				}
			}
			if (end_color_g != "")
			{

				for (int i = 0; i < Alive.size(); i++)
				{
					if (!Alive[i])
					{
						continue;
					}
					float life_progress = static_cast<float> (framesAlive[i]) / duration_frames;

					COL_G[i] = glm::mix(COL_G_S[i], end_color_g_f, life_progress);
				}
			}
			if (end_color_b != "")
			{

				for (int i = 0; i < Alive.size(); i++)
				{
					if (!Alive[i])
					{
						continue;
					}
					float life_progress = static_cast<float> (framesAlive[i]) / duration_frames;

					COL_B[i] = glm::mix(COL_B_S[i], end_color_b_f, life_progress);
				}
			}
			if (end_color_r != "")
			{

				for (int i = 0; i < Alive.size(); i++)
				{
					if (!Alive[i])
					{
						continue;
					}
					float life_progress = static_cast<float> (framesAlive[i]) / duration_frames;
					COL_R[i] = glm::mix(COL_R_S[i], end_color_r_f, life_progress);
				}
			}



		for (int i = 0; i < Alive.size(); i++)
		{
			if (!Alive[i])
			{
				continue;
			}
			std::string tmp = ParticleName;
			if (ParticleName == "") 
			{
				tmp = DefaultParticleName;
			}





			RendererHandler::QueueWorldEXDrawRequests(tmp,POS_X[i],POS_Y[i],ROT_[i], SCALE_[i], SCALE_[i], .5f, .5f, COL_R[i], COL_G[i], COL_B[i], COL_A[i], sorting_order);
		}

		// render
		local_frame_num++;
	}
	void OnDestroy() 
	{

	}

	void DoBurst() 
	{

		if (local_frame_num % frames_between_bursts == 0) 
		{
			int tmp = burst_quantity;

			for (int i = 0; i < tmp; i++)
			{
				float angle_radians = glm::radians(myRandomEngineAngle.Sample());
				float radius = myRandomEngineRadius.Sample();

				float cos_angle = glm::cos(angle_radians);
				float sin_angle = glm::sin(angle_radians);

				float x_start = x + cos_angle * radius;
				float y_start = y + sin_angle * radius;

				float speed = myRandomEngineSpeed.Sample();
				float speed_x_start = cos_angle * speed;
				float speed_y_start = sin_angle * speed;

				if (free_slots.size() > 0)
				{
					int j = free_slots.front();
					free_slots.erase(free_slots.begin());
					Alive[j] = true;
					POS_X[j] = x_start;
					POS_Y[j] = y_start;
					SCALE_[j] = myRandomEngineStartScale.Sample();
					SCALE_S[j] = SCALE_[j];
					ROT_[j] = myRandomEngineRot.Sample();
					COL_R[j] = StartColorR;
					COL_G[j] = StartColorG;
					COL_B[j] = StartColorB;
					COL_A[j] = StartColorA;
					COL_R_S[j] = StartColorR;
					COL_G_S[j] = StartColorG;
					COL_B_S[j] = StartColorB;
					COL_A_S[j] = StartColorA;
					SPEED_X[j] = speed_x_start;
					SPEED_Y[j] = speed_y_start;
					SPEED_ROT_[j] = myRandomEngineRotSpeed.Sample();
					framesAlive[j] = 0;
					// lin
				}
				else
				{


					Alive.push_back(true);
					POS_X.push_back(x_start);
					POS_Y.push_back(y_start);
					SCALE_.push_back(myRandomEngineStartScale.Sample());
					SCALE_S.push_back(SCALE_.back());
					ROT_.push_back(myRandomEngineRot.Sample());
					COL_R.push_back(StartColorR);
					COL_G.push_back(StartColorG);
					COL_B.push_back(StartColorB);
					COL_A.push_back(StartColorA);

					COL_R_S.push_back(StartColorR);
					COL_G_S.push_back(StartColorG);
					COL_B_S.push_back(StartColorB);
					COL_A_S.push_back(StartColorA);

					SPEED_X.push_back(speed_x_start);
					SPEED_Y.push_back(speed_y_start);
					SPEED_ROT_.push_back(myRandomEngineRotSpeed.Sample());
					framesAlive.push_back(0);
				}
			}
		}

	}

	void Stop() 
	{
		Running = false;
	}

	void Play() 
	{
		Running = true;
	}

	void Burst() 
	{
		int tmp = burst_quantity;

		for (int i = 0; i < tmp; i++)
		{
			float angle_radians = glm::radians(myRandomEngineAngle.Sample());
			float radius = myRandomEngineRadius.Sample();

			float cos_angle = glm::cos(angle_radians);
			float sin_angle = glm::sin(angle_radians);

			float x_start = x + cos_angle * radius;
			float y_start = y + sin_angle * radius;

			float speed = myRandomEngineSpeed.Sample();
			float speed_x_start = cos_angle * speed;
			float speed_y_start = sin_angle * speed;

			if (free_slots.size() > 0)
			{
				int j = free_slots.front();
				free_slots.erase(free_slots.begin());
				Alive[j] = true;
				POS_X[j] = x_start;
				POS_Y[j] = y_start;
				SCALE_[j] = myRandomEngineStartScale.Sample();
				SCALE_S[j] = SCALE_[j];
				ROT_[j] = myRandomEngineRot.Sample();
				COL_R[j] = StartColorR;
				COL_G[j] = StartColorG;
				COL_B[j] = StartColorB;
				COL_A[j] = StartColorA;
				COL_R_S[j] = StartColorR;
				COL_G_S[j] = StartColorG;
				COL_B_S[j] = StartColorB;
				COL_A_S[j] = StartColorA;
				SPEED_X[j] = speed_x_start;
				SPEED_Y[j] = speed_y_start;
				SPEED_ROT_[j] = myRandomEngineRotSpeed.Sample();
				framesAlive[j] = 0;

			}
			else
			{
				// linux

				Alive.push_back(true);
				POS_X.push_back(x_start);
				POS_Y.push_back(y_start);
				SCALE_.push_back(myRandomEngineStartScale.Sample());
				SCALE_S.push_back(SCALE_.back());
				ROT_.push_back(myRandomEngineRot.Sample());
				COL_R.push_back(StartColorR);
				COL_G.push_back(StartColorG);
				COL_B.push_back(StartColorB);
				COL_A.push_back(StartColorA);

				COL_R_S.push_back(StartColorR);
				COL_G_S.push_back(StartColorG);
				COL_B_S.push_back(StartColorB);
				COL_A_S.push_back(StartColorA);

				SPEED_X.push_back(speed_x_start);
				SPEED_Y.push_back(speed_y_start);
				SPEED_ROT_.push_back(myRandomEngineRotSpeed.Sample());
				framesAlive.push_back(0);
			}
		}
	}

};

