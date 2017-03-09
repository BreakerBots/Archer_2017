/*
 * HoldButton.h
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#ifndef HOLDBUTTON_H_
#define HOLDBUTTON_H_

#include "WPILib.h"

class HoldButton {
private:
	int joystickButton;

	bool prevState;
	bool state;

public:

	HoldButton (int joyButton);

	void Update (Joystick xbox);

	void SetState (bool newState);

	bool State ();
	bool PrevState ();

};


#endif /* HOLDBUTTON_H_ */
