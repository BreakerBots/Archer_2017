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

#include "Deadband.h"

#include "XBox.h"
#include "Talons.h"

class Winch {

public:
	enum Buttons {
		kWinchTrigger = XBox::LT
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
