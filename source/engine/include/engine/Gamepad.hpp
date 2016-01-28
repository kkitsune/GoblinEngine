#pragma once

#include "Common.hpp"

constexpr int GamepadDeadzoneLeftStick = 7849;
constexpr int GamepadDeadzoneRightStick = 8689;
constexpr int GamepadDeadzoneTrigger = 30;

enum class GamepadButton : int
{
	Up = 0,
	Down = 1,
	Left = 2,
	Right = 3,
	Start = 4,
	Back = 5,
	LeftThumb = 6,
	RightThumb = 7,
	LeftBumper = 8,
	RightBumper = 9,
	A = 12,
	B = 13,
	X = 14,
	Y = 15
};

enum class GamepadTrigger : int
{
	Left = 0,
	Right = 1
};

enum class GamepadStick : int
{
	Left = 0,
	Right = 1
};

/**
 * Initialize the library.
 *
 * This is critical on non-Windows platforms.
 */
extern ENGINE_API void gamepad_init();

/**
 * Shutdown the library.
 *
 * This will release resources allocated by the library internally.
 *
 * This should be called after forking as well.
 */
extern ENGINE_API void gamepad_shutdown();

/**
 * Updates the state of the gamepads.
 *
 * This must be called (at least) once per game loop.
 */
extern ENGINE_API void gamepad_update();

/**
 * Test if a particular gamepad is connected.
 *
 * \param id The device to check.
 * \returns true if the device is connected, false if it is not.
 */
extern ENGINE_API bool gamepad_is_connected(uint8 id);

/**
 * Test if a particular button is being pressed.
 *
 * \param id The device to check.
 * \param button The button to check.
 * \returns true if the button is down, false if it is not.
 */
extern ENGINE_API bool gamepad_button_down(uint8 id, GamepadButton const& button);

/**
 * Test if a particular button has been depressed since the previous call to GamepadUpdate.
 *
 * \param id The device to check.
 * \param button The button to check.
 * \returns true if the button has been pressed, false if it is not or if it was depressed the previous frame.
 */
extern ENGINE_API bool gamepad_button_triggered(uint8 id, GamepadButton const& button);

/**
 * Test if a particular button has been released since the previous call to GamepadUpdate.
 *
 * \param id The device to check.
 * \param button The button to check.
 * \returns true if the button has been released, true if it is down or if it was not down the previous frame.
 */
extern ENGINE_API bool gamepad_button_released(uint8 id, GamepadButton const& button);

/**
 * Get the trigger value (depression magnitude) in its raw form.
 *
 * \param id The device to check.
 * \param trigger The trigger to check.
 * \returns Trigger depression magnitude (0 to 32767).
 */
extern ENGINE_API int gamepad_trigger_value(uint8 id, GamepadTrigger const& trigger);

/**
 * Get the trigger value (depression magnitude) in normalized form.
 *
 * \param id The device to check.
 * \param trigger The trigger to check.
 * \returns Trigger depression magnitude (0 to 1).
 */
extern ENGINE_API float gamepad_trigger_length(uint8 id, GamepadTrigger const& trigger);

/**
 * Test if a trigger is depressed
 *
 * \param id The device to check.
 * \param trigger The trigger to check.
 * \returns true if down, false otherwise.
 */
extern ENGINE_API bool gamepad_trigger_down(uint8 id, GamepadTrigger const& trigger);

/**
 * Test if a trigger is depressed
 *
 * \param id The device to check.
 * \param trigger The trigger to check.
 * \returns true if triggered, false otherwise.
 */
extern ENGINE_API bool gamepad_trigger_triggered(uint8 id, GamepadTrigger const& trigger);

/**
 * Test if a trigger is depressed
 *
 * \param id The device to check.
 * \param trigger The trigger to check.
 * \returns true if released, false otherwise.
 */
extern ENGINE_API bool gamepad_trigger_released(uint8 id, GamepadTrigger const& trigger);

/**
 * Set the rumble motors on/off.
 *
 * To turn off the rumble effect, set values to 0 for both motors.
 *
 * The left motor is the low-frequency/strong motor, and the right motor is the high-frequency/weak motor.
 *
 * \param id The device to update.
 * \param left Left motor strengh (0 to 1).
 * \param right Right motor strengh (0 to 1).
 */
extern ENGINE_API void gamepad_set_rumble(uint8 id, float left, float right);

/**
 * Query the position of an analog stick as raw values.
 *
 * The values retrieved by this function represent the magnitude of the analog
 * stick in each direction.  Note that it shouldn't be possible to get full
 * magnitude in one direction unless the other direction has a magnitude of
 * zero, as the stick has a circular movement range.
 *
 * \param id The device to check.
 * \param stick The stick to check.
 * \returns glm::ivec2 that stores to magnitude of X and Y (-32767 to 32767).
 */
extern ENGINE_API ivec2 gamepad_stick(uint8 id, GamepadStick const& stick);

/**
 * Query the position of an analog stick as normalized values.
 *
 * The values retrieved by this function represent the magnitude of the analog
 * stick in each direction.  Note that it shouldn't be possible to get full
 * magnitude in one direction unless the other direction has a magnitude of
 * zero, as the stick has a circular movement range.
 *
 * \param id The device to check.
 * \param stick The stick to check.
 * \returns glm::vec2 that stores the magnitude of X and Y (-1 to 1).
 */
extern ENGINE_API vec2 gamepad_stick_norm(uint8 id, GamepadStick const& stick);

/**
 * Query the magnitude of an analog stick.
 *
 * This returns the normalized value of the magnitude of the stick.  That is,
 * if the stick is pushed all the way in any direction, it returns 1.0.
 *
 * \param id The device to check.
 * \param stick The stick to check.
 * \returns The magnitude of the stick (0 to 1).
 */
extern ENGINE_API float gamepad_stick_length(uint8 id, GamepadStick const& stick);

/**
 * Query the direction of a stick (in radians).
 *
 * This returns the direction of the stick.  This value is in radians, not
 * degrees.  Zero is to the right, and the angle increases in a
 * counter-clockwise direction.
 *
 * \param id The device to check.
 * \param stick The stick to check.
 * \returns The angle of the stick (0 to 2*PI).
 */
extern ENGINE_API float gamepad_stick_angle(uint8 id, GamepadStick const& stick);
