/*
 * GyroTesting.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: DS-2017
 */

#include <WPILib.h>
#include <CANTalon.h>

#include <ADXRS450_Gyro.h>

#include <AnalogOutput.h>

#include "PIDOutputEffort.h"
#include "I2C.h"

#include "general/XBox.h"

//Lidar Defines
#define LIDAR_ADDR 0x62
#define LIDAR_CONFIG_REGISTER 0x00
#define LIDAR_DISTANCE_REGISTER 0x8f

class GyroTesting: public IterativeRobot {

public:
	typedef ADXRS450_Gyro Gyro;

private:
	Joystick xbox;

	Gyro gyro;
	PIDOutputEffort output;

	PIDController controller;

	DigitalOutput lidar;

	I2C i2c;
	unsigned char distArray[2];
	int distance;

public:
	GyroTesting():
		xbox(0),
		gyro(SPI::kOnboardCS0)
		,output ()
		,controller(0.1,0,0,&gyro,&output)

		,lidar(0)

//		,i2c()
		,i2c(I2C::Port::kMXP, 0x63)
		,distance(1)

	{
//		output.InitTable(NetworkTable::GetTable("Robot")->GetSubTable("turn_effort"));
//		i2c = I2C()

//		controller.InitTable(NetworkTable::GetTable("Robot")->GetSubTable("gyro_controller"));
		controller.SetOutputRange(-1.0,1.0);

	}//Robot Constructor

private:

	void RobotInit(){

		printf("Not Calibrating\n");
//		gyro.Calibrate();

		controller.Enable();

		printf("Done\n");
	}

	void AutonomousInit(){

		gyro.Reset();

	}//Autonomous Init

	void AutonomousPeriodic(){

		float angle = gyro.GetAngle();
		float rate = gyro.GetRate();
		printf("Angle: %4.3f  Rate: %4.3f\n",angle,rate);

		if (SmartDashboard::GetBoolean("DB/Button 0", false)){
			gyro.Reset();
			SmartDashboard::PutBoolean("DB/Button 0", false);
		}

	}//AutonomousPeriodic

	void TeleopInit(){
		//Called once at the start of each operator period
		printf("TELEOP Initialized\n");

//		trigger.Set(false);
		distArray[0] = 255;
		distArray[1] = 255;

		printf("End of Teleop Init\n");
	}//Teleop Init
	void DisabledInit(){
		printf("Disabled TELEOP\n");
	}

	void TeleopPeriodic(){

		float angle = gyro.GetAngle();
		float rate = gyro.GetRate();
//		printf("Angle: %4.3f  Rate: %4.3f\n",angle,rate);

		if (SmartDashboard::GetBoolean("DB/Button 0", false)){
			gyro.Reset();
			SmartDashboard::PutBoolean("DB/Button 0", false);
		}

		UpdateLidar3();

//		printf("Voltage: %4.3f  Value: %4d\n",lidar.GetVoltage(),lidar.GetValue());
//		printf("Period: %f   \n",lidar.GetPeriod());

	}//teleop Periodic


	void UpdateLidar3(){
		static int count255 = 0;

		i2c.Read(0x63, 2, distArray);
		int temp = ((distArray[0] << 8) + distArray[1]);

		if (temp != 255) {
			distance = temp;
			count255 = 0;
		} else {
			count255 += 1;
		}

		printf("Distance: %dcm", distance);
		if (temp == 255) printf("  %d",count255);
		printf("\n");

		Wait(0.3);

	}//Update Lidar 3


	void UpdateLidar(){
//		printf("Polling Lidar...");
//		i2c.Write(LIDAR_CONFIG_REGISTER, 0x04); // Initiate measurement
		i2c.Write(LIDAR_CONFIG_REGISTER, 0x04);

//		Wait(0.04);
//
		unsigned char status[1];
		i2c.Read(0x01, 1, status);
////		printf("Ready?: %s\n",((8 && status[0])==8?"True":"False"));
//
////		printf("Reading Measurement\n");
//		bool response = false;
		bool response = i2c.Read(LIDAR_DISTANCE_REGISTER, 2, distArray); // Read in measurement
		Wait(0.005); // Delay to prevent over polling

		printf("Ready?: %d\tStatus: %s\tDistance: %3d\tResponse: %s\tRawStatus: %d\n",
				status[0],((1 && status[0])==1?"True":"False"),(((int)distArray[0]) << 8) + distArray[1], response?"Bad":"Good", status[0]);
	}

	void UpdateLidar2(){
		i2c.Write(LIDAR_CONFIG_REGISTER, 0x04);

		unsigned char status[1];

		while (status[0] & 1){
			i2c.Read(0x01, 1, status);
		}

		i2c.Read(LIDAR_DISTANCE_REGISTER, 2, distArray);

		printf("Distance: %dcm\n", (distArray[1] << 8) + distArray[0]);
	}


	void TestPeriodic(){
	}

};

//START_ROBOT_CLASS(GyroTesting)
