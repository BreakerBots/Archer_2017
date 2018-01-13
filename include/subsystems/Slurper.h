/*
 * Slurper.h
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#ifndef SLURPER_H_
#define SLURPER_H_

#include "WPILib.h"
#include <ctre/phoenix/MotorControl/CAN/TalonSRX.h>

#include "general/Talons.h"
#include "general/ToggleButton.h"

class Slurper {
private:

	enum Buttons {
		kForwardToggle = XBox::RB
//		,kReverseHold = XBox::
	};

	TalonSRX talon;

	ToggleButton enableButton;
	float intakeEffort;
	float effort;

public:

	Slurper ();
	Slurper (float forwardEffort);

	void Update(Joystick &xbox);

};

#endif /* SLURPER_H_ */