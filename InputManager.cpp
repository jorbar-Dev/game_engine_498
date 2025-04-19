#include "InputManager.h"
#include "LuaToGLM.h"

void InputManager::Init()
{
	for (int code = SDL_SCANCODE_UNKNOWN; code < SDL_NUM_SCANCODES; code++) 
	{
		keyboard_states[static_cast<SDL_Scancode>(code)] = INPUT_STATE_UP;
	}
	for (int i = 1; i <= 3; i++) 
	{
		mouse_button_states[i] = INPUT_STATE_UP;
	}
}
void InputManager::ProcessEvent(const SDL_Event& e)
{
	// Get variables for update checks
	SDL_Scancode code = e.key.keysym.scancode;
	int button = e.button.button;

	// Check scancodes
	if (e.type == SDL_KEYDOWN)
	{
		keyboard_states[code] = INPUT_STATE_JUST_BECAME_DOWN;
		just_became_down_scancodes.push_back(code);
	}
	if (e.type == SDL_KEYUP)
	{
		keyboard_states[code] = INPUT_STATE_JUST_BECAME_UP;
		just_became_up_scancodes.push_back(code);
	}

	// Handle Button Updates
	if (e.type == SDL_MOUSEBUTTONDOWN) 
	{
		mouse_button_states[button] = INPUT_STATE_JUST_BECAME_DOWN;
		just_became_down_buttons.push_back(button);
	}
	if (e.type == SDL_MOUSEBUTTONUP) 
	{
		mouse_button_states[button] = INPUT_STATE_JUST_BECAME_UP;
		just_became_up_buttons.push_back(button);
	}

	// mouse wheel
	if (e.type== SDL_MOUSEWHEEL) 
	{
		mouse_scoll_this_frame += e.wheel.preciseY;
	}

	// set the position of the mouse
	if (e.type == SDL_MOUSEMOTION) 
	{
		mouse_position.x = e.motion.x;
		mouse_position.y = e.motion.y;
	}

}
void InputManager::LateUpdate() 
{
	// reset keys
	for (const SDL_Scancode& code : just_became_down_scancodes) 
	{
		keyboard_states[code] = INPUT_STATE_DOWN;

	}
	just_became_down_scancodes.clear();

	for (const SDL_Scancode& code : just_became_up_scancodes)
	{
		keyboard_states[code] = INPUT_STATE_UP;

	}
	just_became_up_scancodes.clear();

	
	// reset buttons
	for (const int& code : just_became_down_buttons)
	{
		mouse_button_states[code] = INPUT_STATE_DOWN;

	}
	just_became_down_buttons.clear();

	for (const int& code : just_became_up_buttons)
	{
		mouse_button_states[code] = INPUT_STATE_UP;

	}
	just_became_up_buttons.clear();

	// reset mouse scroll
	mouse_scoll_this_frame = 0;
}

bool InputManager::GetKey(std::string LuaInput) 
{
	SDL_Scancode keycode;
	if (__keycode_to_scancode.count(LuaInput) > 0)
	{
		keycode = __keycode_to_scancode.at(LuaInput);
	}
	else 
	{
		return false;
	}
	if (keyboard_states[keycode] == INPUT_STATE_DOWN || keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_DOWN) 
	{
		return true;
	}
	return false;
}
bool InputManager::GetKeyDown(std::string LuaInput)
{
	SDL_Scancode keycode;
	if (__keycode_to_scancode.count(LuaInput) > 0)
	{
		keycode = __keycode_to_scancode.at(LuaInput);
	}
	else
	{
		return false;
	}
	if (keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_DOWN)
	{
		return true;
	}
	return false;
}
bool InputManager::GetKeyUp(std::string LuaInput)
{
	SDL_Scancode keycode;
	if (__keycode_to_scancode.count(LuaInput) > 0)
	{
		keycode = __keycode_to_scancode.at(LuaInput);
	}
	else
	{
		return false;
	}
	if (keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_UP)
	{
		return true;
	}
	return false;
}

glm::vec2 InputManager::GetMousePosition() 
{
	return mouse_position;
}

bool InputManager::GetMouseButton(int button) 
{
	if (mouse_button_states.count(button) == 0)
	{
		return false;
	}
	if (mouse_button_states[button] == INPUT_STATE_DOWN || mouse_button_states[button] == INPUT_STATE_JUST_BECAME_DOWN)
	{
		return true;
	}
	return false;
}
bool InputManager::GetMouseButtonDown(int button) 
{
	if (mouse_button_states.count(button) == 0)
	{
		return false;
	}
	if (mouse_button_states[button] == INPUT_STATE_JUST_BECAME_DOWN)
	{
		return true;
	}
	return false;
}
bool InputManager::GetMouseButtomUp(int button) 
{
	if (mouse_button_states.count(button) == 0)
	{
		return false;
	}
	if (mouse_button_states[button] == INPUT_STATE_JUST_BECAME_UP)
	{
		return true;
	}
	return false;
}
float InputManager::GetMouseScollDelta() 
{
	return mouse_scoll_this_frame;
}