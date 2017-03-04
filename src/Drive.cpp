/*
 * Drive.cpp
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */

#include "WPILib.h"
#include "Drive.h"
#include "XBox.h"
#include "Talons.h"

/*
 * Drive.h
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */


Drive::Drive ():

	driveEnabled(true),
	gearsEnabled(true),

	gearButton(XBox::A),

	right1(Talons::R1),
	right2(Talons::R2),
	right3(Talons::R3),
//	encoderRight(right1),
//	controlRight(0.001,0,0,&encoderRight,&right1),

	left1(Talons::L1),
	left2(Talons::L2),
	left3(Talons::L3),
//	encoderLeft(left1),
//	controlLeft(0.001,0,0,&encoderLeft, &left1),

	directionButton(XBox::B),

	drive (left1, right1),

	moveDeadband(0.1),
	turnDeadband(0.2),

	teleop(true),
	autoAdjustmentValue(0)

	,autoTargetRight(10000),
	autoTargetLeft(-10000)
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

	//----------Set up Encoders------------//
		//Note: SetPID is not ambiguous ... stupid Eclipse
//	right1.SetControlMode(CANTalon::kPosition);
//	right1.SetEncPosition(0);
//	right1.SetIzone(2000);
//	right1.Set(0);
//	right1.SetP(0);
//	right1.SetI(0);
//	right1.SetD(0);

//	left1.SetControlMode(CANTalon::kPosition);
//	left1.SetEncPosition(0);
//	left1.SetIzone(2000);
//	left1.Set(0);
//	left1.SetP(0);
//	left1.SetI(0);
//	left1.SetD(0);

}//Basic Constructor

void Drive::Init (std::shared_ptr<ITable> nt){
	driveTable = nt;

	right1.SetEncPosition(0);
	left1.SetEncPosition(0);

	WritePIDTable();
	drive.SetMaxOutput(1);


}
int count = 0;
void Drive::AutonomousInit(){

//	right1.ConfigPeakOutputVoltage(6,-6);
//	right1.SetVoltageRampRate(12);//Volts per second
//	left1.ConfigPeakOutputVoltage(6,-6);
//	left1.SetVoltageRampRate(12);

	autoTargetRight = 10000;
	autoTargetLeft = -10000;

	count = 0;

}
void Drive::Autonomous(double *izone/* Why would we need a joystick?*/){

	ReadPIDTable();


//	autoTargetRight -= autoAdjustmentValue / 100;
//	autoTargetLeft -= autoAdjustmentValue / 100;
	int pauseSec = 3;
//	right1.Set(autoTargetRight);
//	left1.Set(autoTargetLeft);
	count++;
	if (right1.GetEncPosition() - left1.GetEncPosition() < 10000){
		drive.ArcadeDrive(-0.6,0);
		count = 0;
		*izone = 0;
	} else if (count > pauseSec*50 && right1.GetEncPosition() - left1.GetEncPosition() < 50000){
		drive.ArcadeDrive(-0.6,autoAdjustmentValue);
	} else {
		printf("Count: %d\n",count);
	}

	PostValues();
}

void Drive::Update (const Joystick& xbox){
	//Check autonomous button
	if (xbox.GetRawButton(XBox::Y))
		teleop = false;
	else
		teleop = true;

	ReadPIDTable();

	//Driving Commands
	directionButton.Update(xbox);
	int rev = 1;
	if (directionButton.State()) rev = -1;

	if (driveEnabled){
		if (teleop){

			drive.ArcadeDrive(moveDeadband.OutputFor(rev*xbox.GetRawAxis(XBox::LY)),
							turnDeadband.OutputFor(xbox.GetRawAxis(XBox::LX)));
			printf("Y: %f\n",2000*moveDeadband.OutputFor(rev*xbox.GetRawAxis(XBox::LY)));
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

void Drive::ReadPIDTable (){
	if (driveTable){
		left1.SetP(driveTable->GetNumber("PID/Left P",0));
		left1.SetI(driveTable->GetNumber("PID/Left I",0));
		left1.SetD(driveTable->GetNumber("PID/Left D",0));
		left1.SetF(driveTable->GetNumber("PID/Left F",0));

		right1.SetP(driveTable->GetNumber("PID/Right P",0));
		right1.SetI(driveTable->GetNumber("PID/Right I",0));
		right1.SetD(driveTable->GetNumber("PID/Right D",0));
		right1.SetF(driveTable->GetNumber("PID/Right F",0));
	}
}

void Drive::WritePIDTable (){
	if (!driveTable) return;

	//Velocity Control
	/*
	 * P -- 0.5
	 * I -- 0.002
	 * D -- 0
	 * F -- 0.4
	 *
	 * IZone: 600 (maybe 300?)
	 */

	//Position Control (Straight)
	/*
	 * P -- 0.05
	 * I -- 0.0005
	 * D -- 0.5
	 * F -- 0
	 *
	 * IZone: 2000
	 */


	if (driveTable->GetBoolean("PID/LoadDefault",true)){
		driveTable->PutNumber("PID/Left P",0.5);
		driveTable->PutNumber("PID/Left I",0.002);
		driveTable->PutNumber("PID/Left D",0);
		driveTable->PutNumber("PID/Left F",0.4);

		driveTable->PutNumber("PID/Right P",0.5);
		driveTable->PutNumber("PID/Right I",0.002);
		driveTable->PutNumber("PID/Right D",0);
		driveTable->PutNumber("PID/Right F",0.4);
	}
}

void Drive::PostValues (){
	//Post Values to the SmartDashboard/Subsystems/Drive network table

	driveTable->PutBoolean("1.Drive Enabled",driveEnabled);
	driveTable->PutString("2.Drive Direction",(directionButton.State()?"FORWARD":"REVERSE"));
	driveTable->PutString("3.Control Mode",(teleop?"TELE-OP":"AUTONOMOUS"));
	driveTable->PutNumber("4.AutoAdjust",autoAdjustmentValue);
	driveTable->PutBoolean("5.Gears Enabled",gearsEnabled);
	driveTable->PutString("6.Driving Gear",(gearButton.State()?"HIGH GEAR":"LOW GEAR"));

	//Debug Values: Troubleshoot discrepancy with above values
	driveTable->PutNumber("Debug/1.LeftEffort",left1.Get());
	driveTable->PutNumber("Debug/2.RightEffort",right1.Get());

	driveTable->PutNumber("Debug/3.LeftEncVel",left1.GetEncVel());
	driveTable->PutNumber("Debug/4.RightEncVel",right1.GetEncVel());

	driveTable->PutNumber("Debug/5.LeftEncPos",left1.GetEncPosition());
	driveTable->PutNumber("Debug/6.RightEncPos",right1.GetEncPosition());

	driveTable->PutNumber("Debug/7.LeftGear",leftGear->Get());
	driveTable->PutNumber("Debug/8.RightGear",rightGear->Get());

}
