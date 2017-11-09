/*
 * Drive.cpp
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */

#include "WPILib.h"

#include "general/XBox.h"
#include "general/Talons.h"
#include "ctrlib/CANTalon.h"

#include "subsystems/drive/Drive.h"

/*
 * Drive.h
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */


Drive::Drive (double *izoneFromGearPlacer):

	driveEnabled(true),
	gearsEnabled(true),

	pusherButton(Buttons::kPusherToggle),
	gearButton(Buttons::kGearToggle),

	gearRed(Talons::kRed),

	right1(Talons::R1),
	right2(Talons::R2),
	right3(Talons::R3),

	left1(Talons::L1),
	left2(Talons::L2),
	left3(Talons::L3),

	directionButton(Buttons::kDirectionToggle),

	drive (left1, right1),

	moveDeadband(0.1),
	maxTurnHighGear(1),
	maxTurnLowGear(1),
	turnDeadband(0.1),

	izone(izoneFromGearPlacer),
	autoAim(-1),
	autoAdjustmentValue(0),
	encoderCountsForGear2(0),
	onTarget(false),

	autoState(Drive::AutoState::kStraight),
	autoTimer()
{
	//Start in reverse
	directionButton.Override(!directionButton.State());

	pusher = new DoubleSolenoid(50,6,7);
	gearsLeft = new DoubleSolenoid(50,0,1);
	gearsRight = new DoubleSolenoid(50,4,5);


	//----------Talon Management-----------//
	right2.SetControlMode(CANTalon::kFollower);
	right2.Set(Talons::R1);
	right3.SetControlMode(CANTalon::kFollower);
	right3.Set(Talons::R1);

	left2.SetControlMode(CANTalon::kFollower);
	left2.Set(Talons::L1);
	left3.SetControlMode(CANTalon::kFollower);
	left3.Set(Talons::L1);

	autoTimer.Reset();

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

	autoTimer.Start();
//	drive.SetMaxOutput(650);

}
void Drive::AutonomousInit(AutonomousMode mode){

//	right1.ConfigPeakOutputVoltage(6,-6);
//	right1.SetVoltageRampRate(12);//Volts per second
//	left1.ConfigPeakOutputVoltage(6,-6);
//	left1.SetVoltageRampRate(12);

	right1.SetEncPosition(0);
	left1.SetEncPosition(0);

	autoState = AutoState::kHook;

	pusher->Set(DoubleSolenoid::kReverse);

	if (mode == AutonomousMode::kGyroStraight){
		autoState = AutoState::kForward;

		autoTimer.Reset();
		autoTimer.Start();
	} else if (mode == AutonomousMode::kGear2){
		autoState = AutoState::kForward;
	}

	driveTable->PutNumber("autoState",0);

}
Drive::AutonomousCommand Drive::Autonomous(AutonomousMode autonomousMode/* Why would we need a joystick?*/){
	AutonomousCommand command = AutonomousCommand::kNull;
	ReadPIDTable();

	/*
	 *
	 * Robot Width: 40in.
	 * Robot Length: 36in.
	 *
	 * Distance from Alliance Wall to Baseline: 7' 9 3/4''
	 * Distance form Alliance Wall to LIFT:     9' 3''     or 111in.
	 *
	 * Distance from front of robot to LIFT: 75in.
	 */



	int advanceInches;

	switch (autonomousMode){
	case kDefault:
//		printf("Default Autonomous Mode\n");
		drive.ArcadeDrive(0.0,0);
		break;
	case kBaseline:
		//The total distance from boiler to nearest peg is 130in.
		//We want to come a bit short for safety --> 120in. The
		//front of the robot must then travel 84in. (120in. - 36in.)

		//To travel 75in. in putting the gear on the front peg, a
		//value of 103in. is then scaled into encoder counts. Thus,
		//we scale 84in. up to 115in (84in. * 103/75)

//		advanceInches = 115;
		advanceInches = 50;
		if (left1.GetEncPosition()*2/* + right1.GetEncPosition() No Encoder on Chadwick's right side*/ > -advanceInches*1000/3.32){
			drive.ArcadeDrive(0.4,autoAdjustmentValue);
//			drive.ArcadeDrive(0.5, autoAdjustmentValue);
//			*izone = 0;
//		} else if (right1.GetEncPosition() + left1.GetEncPosition() > -43000){
//			drive.ArcadeDrive(0.5,autoAdjustmentValue);
		} else {
			drive.ArcadeDrive(0.0,0.0);
			printf("Done\n");
		}
		break;
	case kGear1:

		advanceInches = -(112-30);
		if (right1.GetEncPosition() + left1.GetEncPosition() > -10000){
			drive.ArcadeDrive(0.5,0);
			*izone = 0;
		} else if (right1.GetEncPosition() + left1.GetEncPosition() > -50000){
			drive.ArcadeDrive(0.5,autoAdjustmentValue);
		} else {
			drive.ArcadeDrive(0.0,0.0);
			printf("Done\n");
		}
		break;
	case kGear2:

		advanceInches = /*7ft.*/60*(2)    *0.85 *0.9 *0.95  *1.15   *0.95/**1.05*/;
		switch (autoState){
		case kForward:
			drive.ArcadeDrive(0.85, autoAdjustmentValue);

//			if (right1.GetEncPosition() < -60*1000/3.32){
			/* From kBaseline MODIFIED  -- -50,000 --> -43,000*/
			if (left1.GetEncPosition()*2/* + right1.GetEncPosition()No encoder on Chadwick's right side*/< -advanceInches *1000/3.32){
				autoState = kTurn;
				command = AutonomousCommand::kPrepTurn;

				autoTimer.Reset();
				autoTimer.Start();
			}
			break;
		case kTurn:
			drive.ArcadeDrive(0, autoAdjustmentValue);

//			if (driveTable->GetNumber("angle",0) < -55){
//			left1.SetEncPosition(0);
//			if (onTarget && autoTimer.Get() > 0.25){
			if (Delay(2.5*1.5)){
				printf("Closing\n");
				autoState = kClose;
//				left1.SetEncPosition(0);
				encoderCountsForGear2 = 2*left1.GetEncPosition();

				command = AutonomousCommand::kSwitchToDrivePID;
//			} else if (fabs(driveTable->GetNumber("angle",0)-60) < 0.4){
//				if (!onTarget){
//					onTarget = true;
//					autoTimer.Reset();
//					autoTimer.Start();
//				}
//			} else {
//				autoTimer.Reset();
//				onTarget = false;
			}


			break;
		case kClose:
			printf("Driving Straight!\n");
			drive.ArcadeDrive(0.65, autoAdjustmentValue);

			if (left1.GetEncPosition()*2 - encoderCountsForGear2 < (0.7) * -(120)*1000/3.32){
				autoState = kDeposit;
				encoderCountsForGear2 = 2*left1.GetEncPosition();

				command = AutonomousCommand::kOpenWings;

				autoTimer.Reset();
				autoTimer.Start();
			}
			break;
		case kDeposit:
			drive.ArcadeDrive(0.0,0);

			if (Delay(0.5)){
				pusher->Set(DoubleSolenoid::kForward);
				autoState = kDelay;
			}
			break;
		case kDelay:
			if (Delay(0.5)){
				autoState = kReverse;
			}
			break;
		case kReverse:
			drive.ArcadeDrive(-0.5,0);

			if (2*left1.GetEncPosition() - encoderCountsForGear2 > 30*1000/3.32){
//			if (Delay (1)){
//				pusher->Set(DoubleSolenoid::kReverse);
				autoState = kDone;
				command = AutonomousCommand::kCloseWings;
			}
			break;
		case kDone:
			drive.ArcadeDrive(0.0,0);
			printf("Done\n");

			pusher->Set(DoubleSolenoid::kReverse);
			break;
		default:
			printf("Unknown Auto State!!\n");
			break;
		}
		break;
	case kGear3:
		advanceInches = -64;//inches
//		turnDistance = 16.75 * 1000 / 3.32; // ~5,045

		switch (autoState){
		case kHook:
			printf("Hook\n");
			//Todo figure out how to curve archer in autonomous
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
		default:
			printf("Unknown state\n");
			break;
		}//autoState switch
		break;
	case kGyroStraight:

		advanceInches = 103   *1.1;

		switch (autoState){
		case kForward:
			drive.ArcadeDrive(0.65, autoAdjustmentValue);


//			if (Delay(3)){
//			if (right1.GetEncPosition() < -60*1000/3.32){
			/* From kBaseline MODIFIED  -- -50,000 --> -43,000*/
			if (left1.GetEncPosition()*2/* + right1.GetEncPosition()/*No encoder on Chadwick's right side*/< -advanceInches *1000/3.32){

				/*
				 * -advanceInches(103)*1000/3.32 == -31,024
				 * Encoder count for each side == -15,512
				 *
				 * Inches we want to travel =
				 */

				autoState = kDeposit;
				command = AutonomousCommand::kOpenWings;

				autoTimer.Reset();
				autoTimer.Start();
			}
			break;
		case kDeposit:
			drive.ArcadeDrive(0.0,0);

			if (Delay(1)){
				pusher->Set(DoubleSolenoid::kForward);
				autoState = kDelay;
			}
			break;
		case kDelay:
			if (Delay(1.5)){
				autoState = kReverse;
			}
			break;
		case kReverse:
			drive.ArcadeDrive(-0.5,0);

			if (left1.GetEncPosition() > -30*1000/3.32){
//			if (Delay (1)){
//				pusher->Set(DoubleSolenoid::kReverse);
				autoState = kDone;
				command = AutonomousCommand::kCloseWings;
			}
			break;
		case kDone:
			drive.ArcadeDrive(0.0,0);

			pusher->Set(DoubleSolenoid::kReverse);
			break;
		default:
			printf("Unknown Auto State!!\n");
			break;
		}

		break;
	default:
		printf("Unknown autonomous mode\n");
	}

	GearLights();
	PostValues();

	return command;
}

void Drive::Update (Joystick &xbox){

	if (xbox.GetRawAxis(Buttons::kAimingTrigger) > 0.3)
		autoAim.SetState(true);
	else
		autoAim.SetState(false);

//	printf("Adjust: %f\n",autoAdjustmentValue);
	ReadPIDTable();

	int rev = 1;
	directionButton.Update(xbox);
	if (directionButton.State()) rev = -1;

	if (driveEnabled){
		if (autoAim.State()){
			if (!autoAim.PrevState())
				*izone = 0;

			drive.ArcadeDrive(moveDeadband.OutputFor(rev*xbox.GetRawAxis(XBox::LY)),
							autoAdjustmentValue);
		} else {
			drive.ArcadeDrive(moveDeadband.OutputFor(rev*xbox.GetRawAxis(XBox::LY)),
							turnDeadband.OutputFor(xbox.GetRawAxis(XBox::LX)));
//			drive.ArcadeDrive(rev*xbox.GetRawAxis(XBox::LY), xbox.GetRawAxis(XBox::LX));
//			driveTable->PutNumber("Adjusted_X",turnDeadband.OutputFor(xbox.GetRawAxis(XBox::LX)));
//			driveTable->PutNumber("Adjusted_Y",moveDeadband.OutputFor(xbox.GetRawAxis(XBox::LY)));
		}
	}

	//---------PUSHER------------//
	pusherButton.Update(xbox);
	if (pusherButton.State()){
//		if (!pusherButton.PrevState())
			pusher->Set(DoubleSolenoid::kForward);
	} else {
//		if (pusherButton.PrevState())
			pusher->Set(DoubleSolenoid::kReverse);
	}

	//------------GEARS----------//
	gearButton.Update(xbox);
	if (gearsEnabled){
		if (gearButton.State()){
			gearsRight->Set(DoubleSolenoid::kForward);
			gearsLeft->Set(DoubleSolenoid::kForward);

			SmartDashboard::PutBoolean("DB/LED 0",true);

//			if (!gearButton.PrevState()){
//				turnDeadband.SetMaxY(maxTurnHighGear);
//				turnDeadband.SetMinY(-maxTurningHighGear);
//			}
		} else {
			gearsRight->Set(DoubleSolenoid::kReverse);
			gearsLeft->Set(DoubleSolenoid::kReverse);

			SmartDashboard::PutBoolean("DB/LED 0",false);
//			if (gearButton.PrevState()){
//				turnDeadband.SetMaxY(maxTurnLowGear);
//				turnDeadband.SetMinY(-maxTurnLowGear);
//			}
		}
	}

	GearLights();
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

bool Drive::PusherOut(){
	return pusher->Get() == DoubleSolenoid::kForward;
}
void Drive::PushGear(){
	pusherButton.Override(true);
}
void Drive::PullGear(){
	pusherButton.Override(false);
}

void Drive::PIDWrite (double output){
	autoAdjustmentValue = output;
}

void Drive::GearLights(){
	if (gearButton.State()){
		gearRed.Set(1);
	} else {
		gearRed.Set(0);
	}
}//GearLights

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

	if (!driveTable->ContainsKey("AutonomousMode"))
		driveTable->PutNumber("AutonomousMode",0);

	driveTable->PutBoolean("1.Drive Enabled",driveEnabled);
	driveTable->PutString("2.Drive Direction",(directionButton.State()?"FORWARD":"REVERSE"));
	driveTable->PutString("3.Control Mode",(autoAim.State()?"AUTONOMOUS":"TELE-OP"));
	driveTable->PutNumber("4.AutoAdjust",autoAdjustmentValue);
	driveTable->PutBoolean("5.Gears Enabled",gearsEnabled);
	driveTable->PutString("6.Driving Gear",(gearButton.State()?"HIGH GEAR":"LOW GEAR"));

	//Debug Values: Troubleshoot discrepancy with above values
	driveTable->PutNumber("Debug2/0.LeftEffort",left1.Get());
	driveTable->PutNumber("Debug2/1.RightEffort",right1.Get());

	driveTable->PutNumber("Debug2/2.LeftEncVel",left1.GetEncVel());
	driveTable->PutNumber("Debug2/3.RightEncVel",right1.GetEncVel());

	driveTable->PutNumber("Debug2/4.LeftEncPos",left1.GetEncPosition());
	driveTable->PutNumber("Debug2/5.RightEncPos",right1.GetEncPosition());

	driveTable->PutNumber("Debug2/6.GearsLeft",gearsLeft->Get());
	driveTable->PutNumber("Debug2/7.GearsRight",gearsRight->Get());

	driveTable->PutNumber("Debug2/8.MaxSpeedHighGear", maxTurnHighGear);
	driveTable->PutNumber("Debug2/9.MaxSpeedLowGear", maxTurnLowGear);
}

bool Drive::Delay(float delaySeconds){
	if (autoTimer.Get() > delaySeconds){
		autoTimer.Reset();
		autoTimer.Start();
		return true;
	}
	return false;
}//Delay method
