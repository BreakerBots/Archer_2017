/*
 * Drive.h
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */

#ifndef SRC_DRIVE_H_
#define SRC_DRIVE_H_

#include <general/Deadband.h>
#include <general/HoldButton.h>
#include <general/ToggleButton.h>
#include <general/XBox.h>
#include <PIDController.h>
#include <PIDOutput.h>
#include <RobotDrive.h>
#include <Timer.h>
#include <memory>
#include <ctre/Phoenix.h>

namespace frc {
class ADXRS450_Gyro;
class DoubleSolenoid;
class Joystick;
} /* namespace frc */

class Drive : public PIDOutput
{
public:
	enum AutonomousMode {
			kDefault, kGear1, kGear2, kGear3, kBaseline, kGyroStraight
		};
	enum AutoState {
			kHook, kStraight, kTurn, kWait, kClose, kFinal, kDone
			,kForward, kDeposit, kDelay, kReverse
		};

	enum AutonomousCommand {
		kNull,
		kOpenWings, kCloseWings,
		kTurnTo60,
		kSwitchToTurnPID, kSwitchToDrivePID,
		kPrepTurn
		};

	enum Buttons {
		kGearToggle = XBox::B,
		kDirectionToggle = XBox::LB,
		//As the hold button is triggered by an axis,
		//the set value is an illegal button number.
		kAimingHoldNull = -1,
		kAimingTrigger = -1,
		kPusherToggle = XBox::A
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
	WPI_TalonSRX gearRed;

		//3-talon drive for the right side
	WPI_TalonSRX right1;
	WPI_TalonSRX right2;
	WPI_TalonSRX right3;
		//3-talon drive for the left side
	WPI_TalonSRX left1;
	WPI_TalonSRX left2;
	WPI_TalonSRX left3;

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
	PIDController gyroPID;
	//double *izone;//Used by aiming->drive PID controller
	HoldButton autoAim;
			//TurnVal output by the PIDController
	float autoAdjustmentValue;
	int encoderCountsForGear2;
	bool onTarget;

	AutoState autoState;
	Timer autoTimer;

public:

	Drive (ADXRS450_Gyro* robotGyro);

	void Init (std::shared_ptr<ITable> nt, std::shared_ptr<NetworkTable> pixyNt);
	void AutonomousInit(AutonomousMode mode);
	AutonomousCommand Autonomous (AutonomousMode mode);
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

	bool PusherOut();
	void PushGear();
	void PullGear();

	void PIDWrite(double output);

	void GearLights();
	void ResetGyro();

	void ReadPIDTable();
	void WritePIDTable();

	void PostValues();

private:
	bool Delay (float delaySeconds);

};

#endif /* SRC_DRIVE_H_ */
