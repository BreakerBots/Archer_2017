/*
 * Slurper.h
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#ifndef SLURPER_H_
#define SLURPER_H_

#include "WPILib.h"
#include "CANTalon.h"

#include "Talons.h"
#include "ToggleButton.h"

class Slurper {
private:

	enum Buttons {
		kForwardToggle = XBox::Y
//		,kReverseHold = XBox::
	};

	CANTalon talon;

	ToggleButton enableButton;
	float intakeEffort;
	float effort;

public:

	Slurper ();
	Slurper (float forwardEffort);

	void Update(Joystick &xbox);

};

#endif /* SLURPER_H_ */
