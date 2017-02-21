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

#include "Talons.h"
#include "Deadband.h"
#include "ToggleButton.h"

class Drive : public PIDOutput
{
private:

		//NetworkTable @ SmartDashboard/Subsystems/Drive
	std::shared_ptr<ITable> driveTable;

	bool driveEnabled;
	bool gearsEnabled;

		//Solenoids allow shifts between high and low gears
	DoubleSolenoid* leftGear;
	DoubleSolenoid* rightGear;
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
	Deadband turnDeadband;

		//Autonomous Control Settings
			//Controlled by user or autonomous
	bool teleop;
			//TurnVal output by the PIDController
	float autoAdjustmentValue;

public:

	Drive ();

	void Init (std::shared_ptr<ITable> nt);
	void Update (const Joystick& xbox);

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

	void PostValues();
};

#endif /* SRC_DRIVE_H_ */
