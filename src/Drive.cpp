/*
 * Drive.cpp
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */

#include "WPILib.h"
#include "Drive.h"
#include "XBox.h"

/*
 * Drive.h
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */

#include "WPILib.h"
#include "Talons.h"
#include "XBox.h"

Drive::Drive ():
	gearButton(XBox::B),

	rightPos(0),
	right1(Talons::R1),
	right2(Talons::R2),
	right3(Talons::R3),

	leftPos(0),
	left1(Talons::L1),
	left2(Talons::L2),
	left3(Talons::L3),

	drive (left1, right1),

	directionButton(XBox::A),

	moveDeadband(0.1),
	turnDeadband(0.1),

	teleop(true),
	autoAdjustmentValue(0)
{
	rightGear = new DoubleSolenoid(50,2,3);
	leftGear = new DoubleSolenoid(50,0,1);

	//----------Track the Leaders-----------//
	right1.SetControlMode(CANTalon::ControlMode::kPosition);
	right1.SetFeedbackDevice(CANTalon::QuadEncoder);
	right1.SetEncPosition(0);
	right1.SetP(0);
	right1.SetI(0);
	right1.SetD(0);

	left1.SetControlMode(CANTalon::ControlMode::kPosition);
	left1.SetFeedbackDevice(CANTalon::QuadEncoder);
	left1.SetEncPosition(0);
	left1.SetP(0);
	left1.SetI(0);
	left1.SetD(0);

	//----------Enslave the rest------------//
	right2.SetControlMode(CANTalon::kFollower);
	right2.Set(Talons::R1);
	right3.SetControlMode(CANTalon::kFollower);
	right3.Set(Talons::R1);

	left2.SetControlMode(CANTalon::kFollower);
	left2.Set(Talons::L1);
	left3.SetControlMode(CANTalon::kFollower);
	left3.Set(Talons::L1);

}//Basic Constructor

void Drive::Init (std::shared_ptr<ITable> nt){
	driveTable = nt;
}
void Drive::Update (const Joystick& xbox){
	//Check autonomous button
	if (xbox.GetRawButton(XBox::Y))
		teleop = false;
	else
		teleop = true;

	//Check Direction Button
	directionButton.Update(xbox);

	//1 == Forward -1==Reverse
//	int reverse = (directionButton.State()?1:-1);

	//Driving Commands
//	if (teleop){
//		drive.ArcadeDrive(reverse*moveDeadband.OutputFor(xbox.GetRawAxis(XBox::LY)),
//						turnDeadband.OutputFor(xbox.GetRawAxis(XBox::LX)));
//	} else {
//		drive.ArcadeDrive(reverse*moveDeadband.OutputFor(xbox.GetRawAxis(XBox::LY)),
//						autoAdjustmentValue, true);
//	}
	rightPos += moveDeadband.OutputFor(xbox.GetRawAxis(XBox::RY));
	leftPos  += turnDeadband.OutputFor(xbox.GetRawAxis(XBox::LY));

	right1.Set(rightPos);
	left1.Set(leftPos);

	//------------GEARS----------//
	gearButton.Update(xbox);

	if (gearButton.State()){
		leftGear->Set(DoubleSolenoid::kForward);
		rightGear->Set(DoubleSolenoid::kForward);
	} else {
		leftGear->Set(DoubleSolenoid::kReverse);
		rightGear->Set(DoubleSolenoid::kReverse);
	}
	PostValues();

}//Update method

bool Drive::Teleop(){
	return teleop;
}
void Drive::ForceTeleop(){
	teleop = true;
}

bool Drive::Highgear(){
	return gearButton.State();
}
void Drive::ChangeGears(bool newGearState){
	gearButton.Override(newGearState);
}

bool Drive::FacingForward(){
	return directionButton.State();
}
bool Drive::FacingBackward(){
	return !directionButton.State();
}
void Drive::FaceForward(){
	directionButton.Override(true);
}
void Drive::FaceBackward(){
	directionButton.Override(false);
}
void Drive::FaceForward(bool newDirection){
	directionButton.Override(newDirection);
}

void Drive::PIDWrite (double output){
	autoAdjustmentValue = output;
}

void Drive::PostValues (){
	//Post Values to the SmartDashboard/Subsystems/Drive network table

	driveTable->PutString("Control Mode",(teleop?"TELE-OP":"AUTONOMOUS"));
	driveTable->PutString("Gear",(gearButton.State()?"HIGH GEAR":"LOW GEAR"));
	driveTable->PutString("Direction",(directionButton.State()?"FORWARD":"REVERSE"));
	driveTable->PutNumber("AutoAdjust",autoAdjustmentValue);

	//Debug Values: Troubleshoot discrepancy with above values
	driveTable->PutNumber("Debug/LeftEffort",left1.Get());
	driveTable->PutNumber("Debug/RightEffort",right1.Get());
	driveTable->PutNumber("Debug/LeftPos",leftPos);
	driveTable->PutNumber("Debug/RightPos",rightPos);

	driveTable->PutNumber("Debug/LeftGear",leftGear->Get());
	driveTable->PutNumber("Debug/RightGear",rightGear->Get());

}
