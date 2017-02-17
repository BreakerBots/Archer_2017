#include <WPILib.h>
#include <CANTalon.h>

#include <algorithm>

#include "Talons.h"
#include "Wings.h"
#include "XBox.h"
#include "ToggleButton.h"
#include "BreakerVision.h"
#include "Drive.h"

class Chadwick: public IterativeRobot {

private:
	Joystick xbox;

	std::shared_ptr<NetworkTable> subsystems;
	std::shared_ptr<NetworkTable> pixy;

	BreakerVision aiming;
	Drive drive;
	PIDController gearPlacer;
	float real_m_I;

	Wings wings;

	CANTalon winch;
	float winchEffort;
	ToggleButton winchButton;

public:
	Chadwick():
		xbox(0),

		aiming(),
		drive(),
		gearPlacer(0,0,0,&aiming,&drive),
		real_m_I(0),

		wings(),

		winch(Talons::WINCH),
		winchEffort(0),
		winchButton(XBox::RB)

	{
		//----------Initializations---------------//

		pixy = NetworkTable::GetTable("Pixy");
		subsystems = NetworkTable::GetTable("Subsystems");

	}//Robot Constructor

private:

	void RobotInit(){
		//No idea when this is called
	}

	void AutonomousInit(){
		//Called once at the start of each Auto period

	}

	void AutonomousPeriodic(){
		//Called PERIODICALLY during the Auto period
	}

	void TeleopInit(){
		//Called once at the start of each operator period
		printf("TELEOP Initialized\n");

		aiming.Init(pixy);
		drive.Init(subsystems->GetSubTable("Drive"));
		wings.Init(subsystems->GetSubTable("Wings"));

		SmartDashboard::PutBoolean("DB/LED 0",true);
		SmartDashboard::PutBoolean("DB/LED 1",true);

		//Begin the feed from aiming to drive
		gearPlacer.Enable();
		//Different than previous PID systems where the setpoint
		//changes in a stable environment, here, the setpoint is
		//always 0, and BreakerVision returns the error off of that
		//setpoint
		gearPlacer.SetSetpoint(0);

		printf("End of Teleop Init\n");

	}//Teleop Init
	void DisabledInit(){
		printf("Disabled TELEOP\n");
	}

	void TeleopPeriodic(){
		//Called PERIODICALLY during the operator period

		//-----------Drive System------------//

		aiming.Update();
		drive.Update(xbox);

		//Update PID loop
		//PIDController gearPlacer will automatically read error from aiming,
		//calculate controlEffort, and output that value to the drive system.
		gearPlacer.SetPID(SmartDashboard::GetNumber("DB/Slider 0",0),SmartDashboard::GetNumber("DB/Slider 1",0),0);

		if (xbox.GetRawButton(XBox::BACK)){
			gearPlacer.m_totalError = 0;
		}
		gearPlacer.m_totalError = 0;

		//-----------Gear Wings-----------//

		wings.Update(xbox);

		//-------------Winch----------------//

		static Deadband ryDeadband(0.1);
		float ryVal = 0;
		if (winchButton.State()){
			winch.Set(winchEffort);
		} else {
			ryVal = ryDeadband.OutputFor(xbox.GetRawAxis(XBox::RY));
			winch.Set(ryVal);
			SmartDashboard::PutNumber("Winch RY: ",ryVal);
		}
		SmartDashboard::PutNumber("Winch Current Draw",winch.GetOutputCurrent());

		//Toggle Winch Autonomous
		winchButton.Update(xbox);
		if (winchButton.State() && !winchButton.PrevState()) winchEffort = ryVal;


		//-----------------------------------//
	}//teleop Periodic

	void TestPeriodic(){
	}
};

START_ROBOT_CLASS(Chadwick)
