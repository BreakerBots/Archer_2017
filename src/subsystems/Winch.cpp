/*
 * Winch.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#include "WPILib.h"

#include "general/XBox.h"
#include "general/Talons.h"
#include "general/Deadband.h"

#include "subsystems/Winch.h"

Winch::Winch ():
	talon (Talons::WINCH),
	joyTrigger (Buttons::kWinchTrigger),

	deadband(0.1)
{
}

void Winch::Update (const Joystick &xbox){

	float effort = deadband.OutputFor(xbox.GetRawAxis(joyTrigger));


	talon.Set(effort);
	if (effort != 0)
		printf("Winch current draw: %.4f\n",talon.GetOutputCurrent());

}//Update

