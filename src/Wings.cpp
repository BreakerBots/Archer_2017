/*
 * Wings.cpp
 *
 *  Created on: Feb 3, 2017
 *      Author: DS_2016
 */

#include "XBox.h"
#include "Wings.h"
#include "ToggleButton.h"

Wings::Wings ():
	button(XBox::X)
{
	leftWing = new DoubleSolenoid(PCM_ID,4,5);
	leftForward = true;

	rightWing = new DoubleSolenoid(PCM_ID,6,7);
	rightForward = false;

}//Wings constructor

Wings::Wings (int triggerNum):
	button(XBox::X)
{

	leftWing = new DoubleSolenoid(PCM_ID,4,5);
	leftForward = true;

	rightWing = new DoubleSolenoid(PCM_ID,6,7);
	rightForward = false;

	button = ToggleButton(triggerNum);
}//Wings constructor

void Wings::Update(const Joystick& xbox){
	button.Update(xbox);

	if (button.State()){
		leftWing->Set(DoubleSolenoid::kForward);
		rightWing->Set(DoubleSolenoid::kForward);
	} else {
		leftWing->Set(DoubleSolenoid::kReverse);
		rightWing->Set(DoubleSolenoid::kReverse);
	}
}//Update Method

void Wings::SetLeftForward(bool newLeftForward){
	leftForward = newLeftForward;
}
bool Wings::GetLeftForward(){
	return leftForward;
}
void Wings::SetRightForward(bool newRightForward){
	rightForward = newRightForward;
}
bool Wings::GetRightForward(){
	return rightForward;
}

void Wings::SetTriggerButton(int newButton){
	button.ChangeTrigger(newButton);
}
int Wings::GetTriggerButton(){
	return button.TriggerButton();
}

void Wings::Open(){
	button.Override(false);
}
void Wings::Close(){
	button.Override(true);
}
bool Wings::AreClosed(){
	return button.State();
}

