/*
 * Winch.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#include "WPILib.h"
#include "CANTalon.h"

#include "XBox.h"
#include "Talons.h"
#include "Winch.h"

Winch::Winch ():
	talon (Talons::WINCH),
	joyTrigger (Buttons::kWinchTrigger),

	deadband(0.1)
{
}

void Winch::Update (const Joystick &xbox){

	float effort = deadband.OutputFor(xbox.GetRawAxis(joyTrigger));

	talon.Set(effort);
}

