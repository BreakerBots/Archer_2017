#include <IterativeRobot.h>
#include <RobotBase.h>

#include "WPILib.h"
#include "CANTalon.h"

class Shooter: public frc::IterativeRobot {
public:
	Joystick xbox;

	CANTalon angleMotor;
	float pos;
	float set1;
	float set2;

//	CANTalon bottomSpin;
	Talon *bottomSpin;
	CANTalon topSpin;

	DoubleSolenoid* solTest;

	Shooter():
		xbox(0),

		angleMotor(9),
		pos(0),
		set1(0),
		set2(0),

//		bottomSpin(0),
		topSpin(7)
	{
		bottomSpin = new Talon(0);
		angleMotor.SetControlMode(CANTalon::kPosition);
		angleMotor.SetFeedbackDevice(CANTalon::QuadEncoder);
		angleMotor.SetEncPosition(0);
		angleMotor.SetPID(0,0,0);

		solTest = new DoubleSolenoid(50,1,2);
	}

	void RobotInit()
	{
	}

	/*
	 * This autonomous (along with the chooser code above) shows how to select
	 * between different autonomous modes using the dashboard. The sendable
	 * chooser code works with the Java SmartDashboard. If you prefer the
	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
	 * GetString line to get the auto name from the text box below the Gyro.
	 *
	 * You can add additional auto modes by adding additional comparisons to the
	 * if-else structure below with additional strings. If using the
	 * SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit() override {
	}

	void AutonomousPeriodic() {
	}

	void TeleopInit() {
		for (int i=0;i<4;i++){
			SmartDashboard::PutBoolean("DB/LED "+std::to_string(i),true);
		}
	}

	void TeleopPeriodic() {

//		float bSpin = 0;
		float bSpin = 2*SmartDashboard::GetNumber("DB/Slider 0",0)/5-1;
		bottomSpin->Set(bSpin);
//		float tSpin = 0;
		float tSpin = 2*SmartDashboard::GetNumber("DB/Slider 1",0)/5-1;
		topSpin.Set(tSpin);

//		pos += xbox.GetRawAxis/10(1/*LeftY*/);
//		pos = SmartDashboard::GetNumber("DB/Slider 3",angleMotor.GetPosition());
		pos -= 1000*deadBand(xbox.GetRawAxis(1),0.15);
		angleMotor.Set(pos);

		float pGain = SmartDashboard::GetNumber("DB/Slider 3",0);
		float iGain = SmartDashboard::GetNumber("DB/Slider 2",0);
		angleMotor.SetP(pGain);
		angleMotor.SetI(iGain);

		if (xbox.GetRawButton(2)){
			if (xbox.GetRawButton(5)){
				set1 = pos;
			} else {
				pos = set1;
			}
		} else if (xbox.GetRawButton(3)){
			if (xbox.GetRawButton(5)){
				set2 = pos;
			} else {
				pos = set2;
			}
		}

		if (xbox.GetRawButton(1)){
			printf("I Accumulative Cleared!\n");
			angleMotor.ClearIaccum();
		}

		if (xbox.GetRawButton(4)){
			solTest->Set(DoubleSolenoid::kForward);
		} else {
			solTest->Set(DoubleSolenoid::kReverse);
		}


		printf("Bottom: %.2f\tTop: %.2f\t\tGoal: %.2f\tPos: %.2f\tPGain: %f\tIGain: %f\n",
				bSpin,        tSpin,       pos,angleMotor.GetPosition(),pGain,iGain);
		printf("TopVoltage: %.2f\n",topSpin.GetOutputVoltage());

	}//teleop Periodic

	float deadBand(float x, float margin){

		if (x < margin && x > -margin) return 0;

		return x;
	}

	void TestPeriodic() {
	}

};

START_ROBOT_CLASS(Shooter)
