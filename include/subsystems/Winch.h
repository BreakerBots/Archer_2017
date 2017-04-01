/*
 * Winch.h
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#ifndef WINCH_H_
#define WINCH_H_

#include "WPILib.h"
#include "CANTalon.h"

#include "general/XBox.h"
#include "general/Talons.h"

#include "general/Deadband.h"


class Winch {

public:
	enum Buttons {
		kWinchTrigger = XBox::RT
	};

private:

	CANTalon talon;
	int joyTrigger;

	Deadband deadband;

public:

	Winch ();
	void Update (const Joystick &xbox);

};


#endif /* WINCH_H_ */
