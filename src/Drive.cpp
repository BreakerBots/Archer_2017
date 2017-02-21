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

Drive::Drive ():

	driveEnabled(true),
	gearsEnabled(true),

	gearButton(XBox::A),

	right1(Talons::R1),
	right2(Talons::R2),
	right3(Talons::R3),

	left1(Talons::L1),
	left2(Talons::L2),
	left3(Talons::L3),

	directionButton(XBox::B),

	drive (left1, right1),

	moveDeadband(0.1),
	turnDeadband(0.2),

	teleop(true),
	autoAdjustmentValue(0)
{
	rightGear = new DoubleSolenoid(50,0,1);
	leftGear = new DoubleSolenoid(50,4,5);

	//----------Talon Management-----------//
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

	//Driving Commands
	directionButton.Update(xbox);
	int rev = 1;
	if (directionButton.State()) rev = -1;

	if (driveEnabled){
		if (teleop){
			drive.ArcadeDrive(moveDeadband.OutputFor(rev*xbox.GetRawAxis(XBox::LY)),
							turnDeadband.OutputFor(xbox.GetRawAxis(XBox::LX)));
		} else {
			drive.ArcadeDrive(moveDeadband.OutputFor(rev*xbox.GetRawAxis(XBox::LY)),
							autoAdjustmentValue, true);
		}
	}

	//------------GEARS----------//
	gearButton.Update(xbox);

	if (gearsEnabled){
		if (gearButton.State()){
			leftGear->Set(DoubleSolenoid::kForward);
			rightGear->Set(DoubleSolenoid::kForward);
		} else {
			leftGear->Set(DoubleSolenoid::kReverse);
			rightGear->Set(DoubleSolenoid::kReverse);
		}
	}
	PostValues();

}//Update method

bool Drive::Teleop(){
	return teleop;
}
void Drive::ForceTeleop(){
	teleop = true;
}

bool Drive::DrivingForward(){
	return directionButton.State();
}
void Drive::SetDrivingForward(bool newDirection){
	directionButton.Override(newDirection);
}

bool Drive::DriveEnabled(){
	return driveEnabled;
}
void Drive::SetDriveEnabled(bool isDriveEnabled){
	driveEnabled = isDriveEnabled;
}

bool Drive::Highgear(){
	return gearButton.State();
}
void Drive::SetHighGear(bool newGearState){
	gearButton.Override(newGearState);
}
bool Drive::GearsEnabled(){
	return gearsEnabled;
}
void Drive::SetGearsEnabled(bool areGearsEnabled){
	gearsEnabled = areGearsEnabled;
}

void Drive::PIDWrite (double output){
	autoAdjustmentValue = output;
}

void Drive::PostValues (){
	//Post Values to the SmartDashboard/Subsystems/Drive network table

	driveTable->PutBoolean("1.Drive Enabled",driveEnabled);
	driveTable->PutString("2. Drive Direction",(directionButton.State()?"FORWARD":"REVERSE"));
	driveTable->PutString("3.Control Mode",(teleop?"TELE-OP":"AUTONOMOUS"));
	driveTable->PutNumber("4.AutoAdjust",autoAdjustmentValue);
	driveTable->PutBoolean("5.Gears Enabled",gearsEnabled);
	driveTable->PutString("6.Driving Gear",(gearButton.State()?"HIGH GEAR":"LOW GEAR"));

	//Debug Values: Troubleshoot discrepancy with above values
	driveTable->PutNumber("Debug/1.LeftEffort",left1.Get());
	driveTable->PutNumber("Debug/2.RightEffort",right1.Get());

	driveTable->PutNumber("Debug/3.LeftGear",leftGear->Get());
	driveTable->PutNumber("Debug/4.RightGear",rightGear->Get());

}
