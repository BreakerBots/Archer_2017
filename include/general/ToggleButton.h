/*
 * ToggleButton.h
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */

#ifndef SRC_TOGGLEBUTTON_H_
#define SRC_TOGGLEBUTTON_H_

#include "WPILib.h"

#include "general/XBox.h"

class ToggleButton {
private:
	int triggerButton;

	bool prevState;
	bool state;
	bool held;

public:
	ToggleButton (int trigger, bool state);
	ToggleButton (int trigger);

	void Update (const Joystick& xbox);
	bool PrevState();
	bool State ();
	void Override(bool newState);

	int TriggerButton();
	void ChangeTrigger(int newTriggerButton);
};

#endif /* SRC_TOGGLEBUTTON_H_ */
