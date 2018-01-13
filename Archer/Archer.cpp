#include <ADXRS450_Gyro.h>
#include <cscore_oo.h>
#include <CameraServer.h>
#include <general/XBox.h>
#include <IterativeRobot.h>
#include <Joystick.h>
#include <networktables/NetworkTable.h>
#include <PIDController.h>
#include <RobotBase.h>
#include <subsystems/drive/BreakerVision.h>
#include <subsystems/drive/Drive.h>
#include <subsystems/Winch.h>
#include <subsystems/Wings.h>
#include <SmartDashboard/SmartDashboard.h>
#include <SPI.h>
#include <Timer.h>
#include <cstdio>
#include <memory>


//#define SHOOTER

class Archer: public IterativeRobot {

public:
	enum Buttons {
		kIntakeToggle
	};

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

	Wings wings;
//	Slurper slurper;
	Winch winch;

//	ToggleButton forceClose;

#ifdef SHOOTER
	Shooter shooter;
#endif

public:
	Archer():
		xbox(0),

		aiming(),
		gearPlacer(0.025,0.0002,0,&aiming,&drive),
		gearPlacerIZone(100),

		gyro(SPI::kOnboardCS0),

		drive(&gyro),
		autonomousMode(Drive::kGear3),

//		gyroPID(0.2,0.04,0,&gyro, &drive),
//		gyroPID(0,0,0,&gyro, &drive),

		wings (),
//		slurper (),
		winch ()

#ifdef SHOOTER
		,shooter()
#endif

//		,forceClose(Buttons::kIntakeToggle)

	{
		//----------Initializations---------------//
		pixy = NetworkTable::GetTable("GearPixy");
		subsystems = NetworkTable::GetTable("Subsystems");

	}//Robot Constructor

private:

	void RobotInit(){
		printf("RobotInit... ");

		if (subsystems->GetBoolean("cameraEnabled",true))
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

//		gyroPID.Enable();
//		gyroPID.InitTable(subsystems->GetSubTable("Drive")->GetSubTable("Gyro"));

		//Different than previous PID systems where the setpoint
		//changes in a stable environment, here, the setpoint is
		//always 0, and BreakerVision returns the error off of that
		//setpoint
		gearPlacer.SetSetpoint(0);
//		gearPlacer.InitTable(pixy->GetSubTable("PID"));


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
//		gyroPID.SetSetpoint(0);
//		gyroPID.m_totalError = 0;//Clear Accumulated Error
//		gyroPID.Enable();

		wings.Close();
		drive.PullGear();

		printf("Done\n");

		Wait(0.2);
	}//AutonomousInit

	void AutonomousPeriodic(){
		//Called PERIODICALLY during the Auto period
		subsystems->GetSubTable("Drive")->PutNumber("angle",gyro.GetAngle());
		printf("Angle: %.2f\n",gyro.GetAngle());

		aiming.Update();

		switch (drive.Autonomous(autonomousMode)){
		case Drive::kOpenWings:
			wings.Open();
			break;
		case Drive::kCloseWings:
			wings.Close();
			break;

/*
		case Drive::kTurnTo60:
			gyroPID.Reset();
			gyroPID.SetSetpoint(-60);
			gyroPID.Enable();
			break;
		case Drive::kSwitchToTurnPID:
			gyroPID.Reset();
			gyroPID.SetPID(0.09, 0.01, 0);
			gyroPID.Enable();
			break;
		case Drive::kSwitchToDrivePID:
			gyroPID.Reset();
			gyroPID.SetPID(0.2, 0.04, 0);
			gyroPID.Enable();
			break;

		case Drive::kPrepTurn:
			gyroPID.Reset();
//			gyroPID.SetPID(0.02, 0.002, 0);
			gyroPID.SetPID(0.05, 0.002, 0.02);
			gyroPID.SetSetpoint(-60);
			gyroPID.Enable();
			break;
*/

		default:
			//No Command
			break;
		}

		//-----------Gear Wings-----------//
		wings.Update(xbox, true);

		//----------Slurper---------------//
//		slurper.Update(xbox);

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

		wings.Close();

		printf("End of Teleop Init\n");

	}//Teleop Init
	void DisabledInit(){
		printf("Disabled!!!\n");
		drive.ResetGyro();
//		gyroPID.Reset();
	}//DisabledInit

	void TeleopPeriodic(){
		//Called PERIODICALLY during the operator period

//		forceClose.Update(xbox);

		subsystems->PutNumber("angle",gyro.GetAngle());

//		if (forceClose.State()){
//			forceClose.Override(false);//Makes it a hold button
		if (xbox.GetRawButton(XBox::Y)){
			wings.Close();
			drive.PullGear();
		}

		//-----------Drive System------------//
		aiming.Update();
		drive.Update(xbox);

//		printf("Angle: %.2f\n",gyro.GetAngle());
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
//		slurper.Update(xbox);

		//-------------Winch----------------//
		winch.Update(xbox);

		if (SmartDashboard::GetBoolean("DB/Button 0",false)){
			gyro.Calibrate();
			SmartDashboard::PutBoolean("DB/Button 0",false);
		}

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
