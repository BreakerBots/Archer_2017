#include <WPILib.h>
#include <CANTalon.h>

#include <algorithm>

#include "general/XBox.h"
#include "general/Talons.h"

#include "general/ToggleButton.h"

#include "subsystems/drive/BreakerVision.h"
#include "subsystems/drive/Drive.h"

#include "subsystems/Wings.h"
#include "subsystems/Winch.h"
#include "subsystems/Slurper.h"

#include "subsystems/shooter/Shooter.h"


//#define SHOOTER

class Archer: public IterativeRobot {

private:

	Joystick xbox;

	std::shared_ptr<NetworkTable> subsystems;
	std::shared_ptr<NetworkTable> pixy;

	BreakerVision aiming;
	PIDController gearPlacer;
	float gearPlacerIZone;

	Drive drive;
	Drive::AutonomousMode autonomousMode;
	ADXRS450_Gyro gyro;
	PIDController gyroPID;

	Wings wings;
	Slurper slurper;
	Winch winch;

#ifdef SHOOTER
	Shooter shooter;
#endif

public:
	Archer():
		xbox(0),

		aiming(),
		gearPlacer(0.025,0.0002,0,&aiming,&drive),
		gearPlacerIZone(100),

		drive(&gearPlacer.m_totalError),
		autonomousMode(Drive::kGear3),

		gyro(SPI::kOnboardCS0),
		gyroPID(0.2,0.04,0,&gyro, &drive),

		wings (),
		slurper (),
		winch ()

#ifdef SHOOTER
		,shooter()
#endif

	{
		//----------Initializations---------------//
		pixy = NetworkTable::GetTable("GearPixy");
		subsystems = NetworkTable::GetTable("Subsystems");

	}//Robot Constructor

private:

	void RobotInit(){
		printf("RobotInit... ");

		if (subsystems->GetBoolean("cameraEnabled",false))
			CameraServer::GetInstance()->StartAutomaticCapture();

		Init();

		printf("Done\n");
	}

	void Init(){
//		printf("Vision init\n");
		aiming.InitTable(pixy);
//		printf("Vision init done\n");

		drive.Init(subsystems->GetSubTable("Drive"), pixy);
		wings.Init(subsystems->GetSubTable("Wings"));
		wings.Close();

		//Begin the feed from aiming to drive
		printf("Aiming->Drive PIDController Not Enabled\n");
//		gearPlacer.Enable();

		gyroPID.Enable();
		gyroPID.InitTable(subsystems->GetSubTable("Drive")->GetSubTable("Gyro"));

		//Different than previous PID systems where the setpoint
		//changes in a stable environment, here, the setpoint is
		//always 0, and BreakerVision returns the error off of that
		//setpoint
		gearPlacer.SetSetpoint(0);
		gearPlacer.InitTable(pixy->GetSubTable("PID"));


#ifdef SHOOTER
		shooter.InitTable(subsystems->GetSubTable("shooter"));
#endif

	}

	void AutonomousInit(){
		//Called once at the start of each Auto period
		printf("Autonomous Init... ");

		autonomousMode = (Drive::AutonomousMode) (int) subsystems->GetSubTable("Drive")->GetNumber("AutonomousMode",0);
		drive.AutonomousInit(autonomousMode);

		gyro.Reset();
		gyroPID.SetSetpoint(0);
		gyroPID.m_totalError = 0;//Clear Accumulated Error

		printf("Done\n");
	}//AutonomousInit

	void AutonomousPeriodic(){
		//Called PERIODICALLY during the Auto period

		printf("Angle: %.2f\n",gyro.GetAngle());

		aiming.Update();
		drive.Autonomous(autonomousMode);

		//-----------Gear Wings-----------//
		wings.Update(xbox, true);

		//----------Slurper---------------//
		slurper.Update(xbox);

		//-------------Winch----------------//
		winch.Update(xbox);

		//-----------Shooter-------------//
#ifdef SHOOTER
		shooter.Update(xbox);
#endif

		//Update PID loop
		//PIDController gearPlacer will automatically read error from aiming,
		//calculate controlEffort, and output that value to the drive system.
//		gearPlacer.SetPID(SmartDashboard::GetNumber("DB/Slider 0",0),SmartDashboard::GetNumber("DB/Slider 1",0),0);


//		if (gearPlacer.GetError() >= gearPlacerIZone){
//			gearPlacer.m_totalError = 0;
//		}
	}//AutonomousPeriodic

	void TeleopInit(){
		//Called once at the start of each operator period
		printf("TELEOP Initialized\n");


		printf("End of Teleop Init\n");

	}//Teleop Init
	void DisabledInit(){
		printf("Disabled!!!\n");
	}//DisabledInit

	void TeleopPeriodic(){
		//Called PERIODICALLY during the operator period

		//-----------Drive System------------//
		aiming.Update();
		drive.Update(xbox);

		printf("Angle: %.2f\n",gyro.GetAngle());
		//Update PID loop
		//PIDController gearPlacer will automatically read error from aiming,
		//calculate controlEffort, and output that value to the drive system.
//		gearPlacer.SetPID(SmartDashboard::GetNumber("DB/Slider 0",0),SmartDashboard::GetNumber("DB/Slider 1",0),0);


		/*
		if (xbox.GetRawButton(XBox::BACK)){
			gearPlacer.m_totalError = 0;
		}
		if (gearPlacer.GetError() >= gearPlacerIZone){
			gearPlacer.m_totalError = 0;
		}
		*/

		//-----------Gear Wings-----------//
		wings.Update(xbox, false);

		//----------Slurper---------------//
		slurper.Update(xbox);

		//-------------Winch----------------//
		winch.Update(xbox);

		//------------Shooter----------------//
#ifdef SHOOTER
		shooter.Update(xbox);
#endif

		//-----------------------------------//
	}//teleop Periodic

	void TestPeriodic(){
	}

};

START_ROBOT_CLASS(Archer)
