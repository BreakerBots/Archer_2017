/*
 * Slurper.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#include "WPILib.h"

#include "general/XBox.h"
#include "general/Talons.h"

#include "subsystems/Slurper.h"

Slurper::Slurper ():
	talon(Talons::INTAKE),

	enableButton(Buttons::kForwardToggle),
	effort(0)
{
	intakeEffort = -0.7;
}//Default Constructor

Slurper::Slurper (float forwardEffort):
	talon(Talons::INTAKE),

	enableButton(Buttons::kForwardToggle),
	effort(0)
{
	intakeEffort = forwardEffort;
}//Custom intakeEffort constructor


void Slurper::Update (Joystick &xbox){

	enableButton.Update(xbox);

	if (enableButton.State())
		effort = intakeEffort;
	else
		effort = 0;

	talon.Set(effort);
}//Update

