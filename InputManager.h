#pragma once
#ifndef INPUT_H
#define INPUT_H



#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include "unordered_map"
#include <vector>
#include <string>

enum INPUT_STATE {INPUT_STATE_UP, INPUT_STATE_JUST_BECAME_DOWN, INPUT_STATE_DOWN,INPUT_STATE_JUST_BECAME_UP};

class InputManager
{
public:
	static void Init();
	static void ProcessEvent(const SDL_Event& e);
	static void LateUpdate();

	static bool GetKey(std::string LuaInput);
	static bool GetKeyDown(std::string LuaInput);
	static bool GetKeyUp(std::string LuaInput);

	static glm::vec2 GetMousePosition();

	static bool GetMouseButton(int button);
	static bool GetMouseButtonDown(int button);
	static bool GetMouseButtomUp(int button);
	static float GetMouseScollDelta();

private:
	static inline std::unordered_map<SDL_Scancode, INPUT_STATE> keyboard_states;
	static inline std::vector<SDL_Scancode> just_became_down_scancodes;
	static inline std::vector<SDL_Scancode> just_became_up_scancodes;

	static inline glm::vec2 mouse_position;
	static inline std::unordered_map<int, INPUT_STATE> mouse_button_states;
	static inline std::vector<int> just_became_down_buttons;
	static inline std::vector<int> just_became_up_buttons;

	static inline float mouse_scoll_this_frame = 0;

};


#endif // !1
