/*
 * Portions of this file are copyright Rebirth contributors and licensed as
 * described in COPYING.txt.
 * Portions of this file are copyright Parallax Software and licensed
 * according to the Parallax license below.
 * See COPYING.txt for license details.

THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.
COPYRIGHT 1993-1999 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/

/*
 *
 * Prototypes for reading controls
 *
 */

#pragma once

#include <type_traits>
#include "joy.h"
#include "dxxsconf.h"

#ifdef __cplusplus
#include <vector>
#include "fwd-event.h"
#include "compiler-array.h"

#ifdef dsx
namespace dcx {

struct control_info
{
	template <typename T>
	struct ramp_controls_t
	{
		T	key_pitch_forward,
			key_pitch_backward,
			key_heading_left,
			key_heading_right,
			key_slide_left,
			key_slide_right,
			key_slide_up,
			key_slide_down,
			key_bank_left,
			key_bank_right;
	};
	struct fire_controls_t
	{
		uint8_t fire_primary, fire_secondary, fire_flare, drop_bomb;
	};
	struct state_controls_t :
		public fire_controls_t,
		public ramp_controls_t<uint8_t>
	{
		uint8_t btn_slide_left, btn_slide_right,
			btn_slide_up, btn_slide_down,
			btn_bank_left, btn_bank_right,
			slide_on, bank_on,
			accelerate, reverse,
			cruise_plus, cruise_minus, cruise_off,
			rear_view,
			automap,
			cycle_primary, cycle_secondary, select_weapon;
	};
	ramp_controls_t<float> down_time; // to scale movement depending on how long the key is pressed
	fix pitch_time, vertical_thrust_time, heading_time, sideways_thrust_time, bank_time, forward_thrust_time;
        fix excess_pitch_time, excess_vertical_thrust_time, excess_heading_time, excess_sideways_thrust_time, excess_bank_time, excess_forward_thrust_time;
};

}

namespace dsx {

struct control_info : ::dcx::control_info
{
#if defined(DXX_BUILD_DESCENT_II)
	struct state_controls_t : ::dcx::control_info::state_controls_t
	{
		uint8_t toggle_bomb,
			afterburner, headlight, energy_to_shield;
	};
#endif
	state_controls_t state; // to scale movement for keys only we need them to be separate from joystick/mouse buttons
	array<fix, 3> mouse_axis, raw_mouse_axis;
#if DXX_MAX_AXES_PER_JOYSTICK
	array<fix, JOY_MAX_AXES> joy_axis, raw_joy_axis;
#endif
};

extern control_info Controls;

}
#endif

#define CONTROL_USING_JOYSTICK	1
#define CONTROL_USING_MOUSE		2
#define MOUSEFS_DELTA_RANGE 512
#ifdef dsx
namespace dsx {
#if defined(DXX_BUILD_DESCENT_I)
constexpr std::integral_constant<unsigned, 50> MAX_CONTROLS{};
#elif defined(DXX_BUILD_DESCENT_II)
constexpr std::integral_constant<unsigned, 60> MAX_CONTROLS{};		// there are actually 48, so this leaves room for more
#endif
}
namespace dcx {
constexpr std::integral_constant<unsigned, 30> MAX_DXX_REBIRTH_CONTROLS{};
extern const array<uint8_t, MAX_DXX_REBIRTH_CONTROLS> DefaultKeySettingsRebirth;
}
#endif

extern void kconfig_read_controls(const d_event &event, int automap_flag);

enum class kconfig_type
{
	keyboard,
#if DXX_MAX_JOYSTICKS
	joystick,
#endif
	mouse,
	rebirth,
};

void kconfig(kconfig_type n);

extern void kc_set_controls();

//set the cruise speed to zero
extern void reset_cruise(void);

extern fix Cruise_speed;


#if DXX_MAX_JOYSTICKS
template <std::size_t N>
struct joystick_text_length : std::integral_constant<std::size_t, joystick_text_length<N / 10>::value + (N >= 10)>
{
};
template <>
struct joystick_text_length<0> : std::integral_constant<std::size_t, 1>
{
};

template <std::size_t N>
class joystick_text_t : std::vector<array<char, N>>
{
	typedef std::vector<array<char, N> > vector_type;
public:
	using vector_type::clear;
	using vector_type::size;
	using vector_type::resize;
	typename vector_type::reference operator[](typename vector_type::size_type s)
	{
		return this->at(s);
	}
};

#if DXX_MAX_AXES_PER_JOYSTICK
using joyaxis_text_t = joystick_text_t<sizeof("J A") + joystick_text_length<DXX_MAX_JOYSTICKS>::value + joystick_text_length<DXX_MAX_AXES_PER_JOYSTICK>::value>;
extern joyaxis_text_t joyaxis_text;
#endif

#if DXX_MAX_BUTTONS_PER_JOYSTICK || DXX_MAX_HATS_PER_JOYSTICK || DXX_MAX_AXES_PER_JOYSTICK
#define DXX_JOY_MAX(A,B)	((A) < (B) ? (B) : (A))
using joybutton_text_t = joystick_text_t<joystick_text_length<DXX_MAX_JOYSTICKS>::value + DXX_JOY_MAX(DXX_JOY_MAX(sizeof("J H ") + joystick_text_length<DXX_MAX_HATS_PER_JOYSTICK>::value, sizeof("J B") + joystick_text_length<DXX_MAX_BUTTONS_PER_JOYSTICK>::value), sizeof("J -A") + joystick_text_length<DXX_MAX_AXES_PER_JOYSTICK>::value)>;
#undef DXX_JOY_MAX
extern joybutton_text_t joybutton_text;
#endif

#endif
#endif
