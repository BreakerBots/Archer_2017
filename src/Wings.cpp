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
	wingsEnabled(true),
	button(XBox::X)
{
	leftWing = new DoubleSolenoid(PCM_ID,6,7);
	leftForward = true;

	rightWing = new DoubleSolenoid(PCM_ID,2,3);
	rightForward = false;

}//Wings constructor

Wings::Wings (int triggerNum):
	wingsEnabled(true),
	button(XBox::X)
{

	leftWing = new DoubleSolenoid(PCM_ID,4,5);
	leftForward = true;

	rightWing = new DoubleSolenoid(PCM_ID,6,7);
	rightForward = false;

	button = ToggleButton(triggerNum);
}//Wings constructor

void Wings::Init(std::shared_ptr<ITable> nt){
	wingsTable = nt;
}

void Wings::Update(const Joystick& xbox){
	button.Update(xbox);

	if (wingsEnabled){
		if (button.State()){
			leftWing->Set(DoubleSolenoid::kForward);
			rightWing->Set(DoubleSolenoid::kForward);
		} else {
			leftWing->Set(DoubleSolenoid::kReverse);
			rightWing->Set(DoubleSolenoid::kReverse);
		}
	}
	PostValues();
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

bool Wings::WingsEnabled(){
	return wingsEnabled;
}
void Wings::SetWingsEnabled(bool areWingsEnabled){
	wingsEnabled = areWingsEnabled;
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

void Wings::PostValues(){
	wingsTable->PutNumber("PCM_ID",PCM_ID);

	wingsTable->PutString("State",(button.State()?"OPEN":"CLOSED"));
	wingsTable->PutNumber("Debug/LeftWing",leftWing->Get());
	wingsTable->PutNumber("Debug/RightWing",rightWing->Get());
}//PostValues method

