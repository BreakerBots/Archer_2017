/*
 * Wings.cpp
 *
 *  Created on: Feb 3, 2017
 *      Author: DS_2016
 */

#include "general/XBox.h"
#include "subsystems/Wings.h"
#include "general/ToggleButton.h"

Wings::Wings ():
	wingsEnabled(true),
	button(XBox::X,true)
{
	cylinder = new DoubleSolenoid(PCM_ID,2,3);

}//Wings constructor

Wings::Wings (int triggerNum):
	wingsEnabled(true),
	button(triggerNum, true)
{
	cylinder = new DoubleSolenoid(PCM_ID,2,3);

}//Wings constructor

void Wings::Init(std::shared_ptr<ITable> nt){
	wingsTable = nt;
}

void Wings::Update(Joystick &xbox){
	button.Update(xbox);

	if (wingsEnabled){
		if (!button.State()){
			cylinder->Set(DoubleSolenoid::kForward);
		} else {
			cylinder->Set(DoubleSolenoid::kReverse);
		}
	}
	PostValues();
}//Update Method

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

	wingsTable->PutString("State",(button.State()?"CLOSED":"OPEN"));
	wingsTable->PutNumber("Debug/CylinderState",cylinder->Get());
}//PostValues method

