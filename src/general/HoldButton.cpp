/*
 * HoldButton.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#ifndef HOLDBUTTON_CPP_
#define HOLDBUTTON_CPP_

#include "WPILib.h"

#include "general/HoldButton.h"

HoldButton::HoldButton(int joyButton):
	joystickButton (joyButton),

	prevState (false),
	state (false)
{
}

void HoldButton::Update(Joystick xbox){
	prevState = state;

	state = xbox.GetRawButton (joystickButton);

}

void HoldButton::SetState (bool newState){
	prevState = state;
	state = newState;
}
bool HoldButton::PrevState(){
	return prevState;
}
bool HoldButton::State (){
	return state;
}


#endif /* HOLDBUTTON_CPP_ */
