/*
 * Winch.h
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#ifndef WINCH_H_
#define WINCH_H_

#include "WPILib.h"
#include <ctre/phoenix/MotorControl/CAN/WPI_TalonSRX.h>

#include "general/XBox.h"
#include "general/Talons.h"

#include "general/Deadband.h"


class Winch {

public:
	enum Buttons {
		kWinchTrigger = XBox::RT
	};

private:

	TalonSRX talon;
	int joyTrigger;

	Deadband deadband;

public:

	Winch ();
	void Update (const Joystick &xbox);

};


#endif /* WINCH_H_ */
