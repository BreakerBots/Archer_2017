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


Drive::Drive (double *izoneFromGearPlacer):

	driveEnabled(true),
	gearsEnabled(true),

	gearButton(Buttons::kGearToggle),

	right1(Talons::R1),
	right2(Talons::R2),
	right3(Talons::R3),

	left1(Talons::L1),
	left2(Talons::L2),
	left3(Talons::L3),

	directionButton(Buttons::kDirectionToggle),

	drive (left1, right1),

	moveDeadband(0.1),
	turnDeadband(0.2),

	izone(izoneFromGearPlacer),
	autoAim(-1),
	autoAdjustmentValue(0),

	autoState(Drive::AutoState::kStraight),
	autoTimer()
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
//	right1.SetControlMode(CANTalon::kSpeed);
//	right1.SetEncPosition(0);
//	right1.SetIzone(2000);
//	right1.Set(0);
//	right1.SetP(0);
//	right1.SetI(0);
//	right1.SetD(0);
//
//	left1.SetControlMode(CANTalon::kSpeed);
//	left1.SetEncPosition(0);
//	left1.SetIzone(2000);
//	left1.Set(0);
//	left1.SetP(0);
//	left1.SetI(0);
//	left1.SetD(0);

}//Basic Constructor

void Drive::Init (std::shared_ptr<ITable> nt, std::shared_ptr<NetworkTable> pixyNt){
	driveTable = nt;
	pixyTable = pixyNt;

	right1.SetEncPosition(0);
	left1.SetEncPosition(0);

	ReadPIDTable();
	WritePIDTable();
	drive.SetMaxOutput(1);
//	drive.SetMaxOutput(650);


}
void Drive::AutonomousInit(){

//	right1.ConfigPeakOutputVoltage(6,-6);
//	right1.SetVoltageRampRate(12);//Volts per second
//	left1.ConfigPeakOutputVoltage(6,-6);
//	left1.SetVoltageRampRate(12);

	right1.SetEncPosition(0);
	left1.SetEncPosition(0);

	autoState = AutoState::kHook;


}
void Drive::Autonomous(AutonomousMode autonomousMode/* Why would we need a joystick?*/){

	ReadPIDTable();

	int advanceInches;

	switch (autonomousMode){
	case kDefault:
		printf("Default Autonomous Mode\n");
		break;
	case kGear1:

		advanceInches = -(112-30);
		if (right1.GetEncPosition() - left1.GetEncPosition() > -10000){
			drive.ArcadeDrive(0.5,0);
			*izone = 0;
		} else if (right1.GetEncPosition() - left1.GetEncPosition() > -50000){
			drive.ArcadeDrive(0.5,autoAdjustmentValue);
		} else {
			drive.ArcadeDrive(0.0,0.0);
//			printf("Count: %d\n",count);
		}
		break;
	case kGear2:
		break;
	case kGear3:
		advanceInches = -64;//inches
//		turnDistance = 16.75 * 1000 / 3.32; // ~5,045

		switch (autoState){
		case kHook:
			printf("Hook\n");
			//Todo
			//Chadwick Straight Back (1000, -1030)
			//1000/sec 900/sec
//			right1.Set(-515);
//			left1.Set(700);

//			drive.ArcadeDrive(0.65,-0.15);
			drive.ArcadeDrive(0.65,-0.4);

			if (pixyTable->GetNumber("TargetData/targetting_number",0) == 2){
				autoState = AutoState::kClose;
				*izone = 0;
			}
			break;
		case kStraight:
			printf("Straight\n");
			drive.ArcadeDrive(0.65,0);

			if ((right1.GetEncPosition() - left1.GetEncPosition())/2 < advanceInches * 1000 / 3.32){
				autoState = AutoState::kTurn;
				driveTable->PutNumber("RCount",right1.GetEncPosition());
			}
			break;
		case kTurn:
			printf("Turn\n");
			drive.ArcadeDrive(0,pixyTable->GetNumber("TargetData/turnEffort",-0.48));

//			if (right1.GetEncPosition() > driveTable->GetNumber("RCount",0) + turnDistance){
			if (pixyTable->GetNumber("TargetData/targetting_number",0) == 2){
				autoState = AutoState::kWait;
				autoTimer.Reset();
				autoTimer.Start();
				*izone = 0;
			}
			break;
		case kWait:
			printf("Wait\n");
			drive.ArcadeDrive(0,0.0);

//			if (pixyTable->GetNumber("TargetData/error",25) < driveTable->GetNumber("TargetTolerance",20)){
			if (autoTimer.Get() > 2.0){
				autoState = AutoState::kClose;
			}
			break;
		case kClose:
			printf("Close\n");
			drive.ArcadeDrive(0.5,autoAdjustmentValue);

//			if (right1.GetEncPosition() < driveTable->GetNumber("RCount",0) - (35*1000/3.32)) {
			if (pixyTable->GetNumber("TargetData/measured_distance",300) < pixyTable->GetNumber("TargetData/target_distance_closer",0)){
				autoState = AutoState::kFinal;
				driveTable->PutNumber("RCount",right1.GetEncPosition());
			}
			break;
		case kFinal:
			printf("Final\n");
			drive.ArcadeDrive(0.5,0);

			if (right1.GetEncPosition() < driveTable->GetNumber("RCount",0) - (10*1000/3.32)) {
				autoState = AutoState::kDone;
			}
			break;
		case kDone:
			drive.ArcadeDrive(0.0,0);
			break;
		}//autoState switch
		break;
	default:
		printf("Unknown autonomous mode\n");
	}

	PostValues();
}

void Drive::Update (const Joystick &xbox){

	if (xbox.GetRawAxis(Buttons::kAimingTrigger) > 0.5)
		autoAim.SetState(true);
	else
		autoAim.SetState(false);


	ReadPIDTable();

	int rev = 1;
	directionButton.Update(xbox);
	if (directionButton.State()) rev = -1;

	if (driveEnabled){
		if (autoAim.State()){
			drive.ArcadeDrive(moveDeadband.OutputFor(rev*xbox.GetRawAxis(XBox::LY)),
							turnDeadband.OutputFor(xbox.GetRawAxis(XBox::LX)));
		} else {
			if (!autoAim.PrevState())
				*izone = 0;

			drive.ArcadeDrive(moveDeadband.OutputFor(rev*xbox.GetRawAxis(XBox::LY)),
							autoAdjustmentValue);
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
	return autoAim.State();
}
void Drive::ForceTeleop(){
	autoAim.SetState(true);
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

	//Aiming PID
	/*
	 * P -- 0.025
	 * I -- 2xE-4
	 * D -- 0
	 * F -- 0
	 *
	 * IZone: 100
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
	driveTable->PutString("3.Control Mode",(autoAim.State()?"AUTONOMOUS":"TELE-OP"));
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
