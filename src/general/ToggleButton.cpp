/*
 * ToggleButton.cpp
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */

#include "Joystick.h"

#include "general/XBox.h"
#include "general/ToggleButton.h"

ToggleButton::ToggleButton(int trigger, bool startingState){
	triggerButton = trigger;

	prevState = startingState;
	state = startingState;

	held = false;
}

ToggleButton::ToggleButton(int trigger){
	triggerButton = trigger;

	prevState = false;
	state = false;

	held = false;
}

void ToggleButton::Update(const Joystick& xbox){
	prevState = state;

	if (xbox.GetRawButton(triggerButton)){
		if (!held){
			held = true;
			state = !state;
		}
	} else {
		held = false;
	}
}

bool ToggleButton::PrevState (){
	return prevState;
}
bool ToggleButton::State (){
	return state;
}
void ToggleButton::Override(bool newState){
	state = newState;
}

int ToggleButton::TriggerButton(){
	return triggerButton;
}
void ToggleButton::ChangeTrigger(int newTriggerButton){
	triggerButton = newTriggerButton;
}

