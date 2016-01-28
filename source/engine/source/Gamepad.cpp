#include <Engine/Gamepad.hpp>
#include <stdexcept>

#include "gamepad/gamepad.h"

using namespace glm;

void gamepad_init()
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
#else
	GamepadInit();
#endif
}

void gamepad_shutdown()
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
#else
	GamepadShutdown();
#endif
}

void gamepad_update()
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
#else
	GamepadUpdate();
#endif
}

bool gamepad_is_connected(uint8 id)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
	return false;
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	return GamepadIsConnected((GAMEPAD_DEVICE) id) == GAMEPAD_TRUE;
#endif
}

bool gamepad_button_down(uint8 id, GamepadButton const& button)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
	return false;
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	return GamepadButtonDown((GAMEPAD_DEVICE) id, (GAMEPAD_BUTTON) button) == GAMEPAD_TRUE;
#endif
}

bool gamepad_button_triggered(uint8 id, GamepadButton const& button)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
	return false;
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	return GamepadButtonTriggered((GAMEPAD_DEVICE) id, (GAMEPAD_BUTTON) button) == GAMEPAD_TRUE;
#endif
}

bool gamepad_button_released(uint8 id, GamepadButton const& button)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
	return false;
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	return GamepadButtonReleased((GAMEPAD_DEVICE) id, (GAMEPAD_BUTTON) button) == GAMEPAD_TRUE;
#endif
}

int gamepad_trigger_value(uint8 id, GamepadTrigger const& trigger)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
	return 0;
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	return GamepadTriggerValue((GAMEPAD_DEVICE) id, (GAMEPAD_TRIGGER) trigger);
#endif
}

float gamepad_trigger_length(uint8 id, GamepadTrigger const& trigger)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
	return 0.f;
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	return GamepadTriggerLength((GAMEPAD_DEVICE) id, (GAMEPAD_TRIGGER) trigger);
#endif
}

bool gamepad_trigger_down(uint8 id, GamepadTrigger const& trigger)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
	return false;
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	return GamepadTriggerDown((GAMEPAD_DEVICE) id, (GAMEPAD_TRIGGER) trigger) == GAMEPAD_TRUE;
#endif
}

bool gamepad_trigger_triggered(uint8 id, GamepadTrigger const& trigger)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
	return false;
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	return GamepadTriggerTriggered((GAMEPAD_DEVICE) id, (GAMEPAD_TRIGGER) trigger) == GAMEPAD_TRUE;
#endif
}

bool gamepad_trigger_released(uint8 id, GamepadTrigger const& trigger)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
	return false;
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	return GamepadTriggerReleased((GAMEPAD_DEVICE) id, (GAMEPAD_TRIGGER) trigger) == GAMEPAD_TRUE;
#endif
}

void gamepad_set_rumble(uint8 id, float left, float right)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	GamepadSetRumble((GAMEPAD_DEVICE) id, left, right);
#endif
}

ivec2 gamepad_stick(uint8 id, GamepadStick const& stick)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
	return {};
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	ivec2 ret;
	GamepadStickXY((GAMEPAD_DEVICE) id, (GAMEPAD_STICK) stick, &ret.x, &ret.y);
	return ret;
#endif
}

vec2 gamepad_stick_norm(glm::uint8 id, GamepadStick const& stick)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
	return {};
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	vec2 ret;
	GamepadStickNormXY((GAMEPAD_DEVICE) id, (GAMEPAD_STICK) stick, &ret.x, &ret.y);
	return ret;
#endif
}

float gamepad_stick_length(glm::uint8 id, GamepadStick const& stick)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
	return 0.f;
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	return GamepadStickLength((GAMEPAD_DEVICE) id, (GAMEPAD_STICK) stick);
#endif
}

float gamepad_stick_angle(glm::uint8 id, GamepadStick const& stick)
{
#ifdef GOBLIN_NO_GAMEPAD
	LOG(FATAL) << "No Gamepad support for MacOS";
	return 0.f;
#else
	if(id >= GAMEPAD_COUNT) throw std::logic_error("Gamepad ID must be between 0 and 3");
	return GamepadStickAngle((GAMEPAD_DEVICE) id, (GAMEPAD_STICK) stick);
#endif
}
