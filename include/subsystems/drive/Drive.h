/*
 * Drive.h
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */

#ifndef SRC_DRIVE_H_
#define SRC_DRIVE_H_

#include "WPILib.h"
#include "CANTalon.h"

#include "general/XBox.h"
#include "general/Talons.h"
#include "general/Deadband.h"
#include "general/ToggleButton.h"
#include "general/HoldButton.h"

class Drive : public PIDOutput
{
public:
	enum AutonomousMode {
			kDefault, kGear1, kGear2, kGear3, kBaseline, kGyroStraight
		};
	enum AutoState {
			kHook, kStraight, kTurn, kWait, kClose, kFinal, kDone
			,kForward, kDeposit, kReverse
		};

	enum Buttons {
		kGearToggle = XBox::A,
		kDirectionToggle = XBox::LB,
		//As the hold button is triggered by an axis,
		//the set value is an illegal button number.
		kAimingHoldNull = -1,
		kAimingTrigger = XBox::RT,
		kPusherToggle = XBox::B
	};

private:

		//NetworkTable @ SmartDashboard/Subsystems/Drive
	std::shared_ptr<ITable> driveTable;
	std::shared_ptr<NetworkTable> pixyTable;

	bool driveEnabled;
	bool gearsEnabled;

		//Gear Push
	DoubleSolenoid* pusher;
	ToggleButton pusherButton;

		//Solenoids allow shifts between high and low gears
	DoubleSolenoid* gearsLeft;
	DoubleSolenoid* gearsRight;
	ToggleButton gearButton;

		//3-talon drive for the right side
	CANTalon right1;
	CANTalon right2;
	CANTalon right3;
		//3-talon drive for the left side
	CANTalon left1;
	CANTalon left2;
	CANTalon left3;

	ToggleButton directionButton;

		//WPILib control function (ArcadeDrive(float moveVal, float turnVal)
	RobotDrive drive;

		//Deadband Functions
	Deadband moveDeadband;
	double maxTurnHighGear;
	double maxTurnLowGear;
	Deadband turnDeadband;

		//Autonomous Control Settings
			//Controlled by user or autonomous
	double *izone;//Used by aiming->drive PID controller
	HoldButton autoAim;
			//TurnVal output by the PIDController
	float autoAdjustmentValue;

	AutoState autoState;
	Timer autoTimer;

public:

	Drive (double *izone);

	void Init (std::shared_ptr<ITable> nt, std::shared_ptr<NetworkTable> pixyNt);
	void AutonomousInit(AutonomousMode mode);
	void Autonomous (AutonomousMode mode);
	void Update (Joystick &xbox);

	bool Teleop();
	void ForceTeleop();

	bool DrivingForward();
	void SetDrivingForward(bool newDirection);

	bool DriveEnabled();
	void SetDriveEnabled(bool isDriveEnabled);

	bool Highgear();
	void SetHighGear(bool newGearState);

	bool GearsEnabled();
	void SetGearsEnabled(bool areGearsEnabled);

	void PIDWrite(double output);

	void ReadPIDTable();
	void WritePIDTable();

	void PostValues();

private:
	bool Delay (float delaySeconds);

};

#endif /* SRC_DRIVE_H_ */
