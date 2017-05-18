/*
 * GyroTesting.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: DS-2017
 */

#include <WPILib.h>
#include <CANTalon.h>

#include "I2C.h"

#include "general/XBox.h"

//Lidar Defines
#define LIDAR_ADDR 0x62
#define LIDAR_CONFIG_REGISTER 0x00
#define LIDAR_DISTANCE_REGISTER 0x8f

class LidarOnBoard: public IterativeRobot {

private:
	I2C i2c;
	unsigned char distArray[2];
	int distance;

public:
	LidarOnBoard():
		i2c(I2C::Port::kOnboard, LIDAR_ADDR),
		distance(1)
	{
	}//Robot Constructor

private:
	void UpdateLidar(){
		static int measurement = 0;
//		printf("Communicating with Lidar\n");
		if (measurement % 100 == 0){
			//Take distance measurement with receiver bias correction
			i2c.Write(LIDAR_CONFIG_REGISTER, 0x04);
		} else {
			//Take distance measurement without receiver bias correction
			i2c.Write(LIDAR_CONFIG_REGISTER, 0x03);
		}
//		printf("Requested Distance Measurement...\n");

		std::uint8_t status;

		std::uint8_t statusRegister = 0x01;
		do {
			i2c.WriteBulk(&statusRegister, 1);
			i2c.ReadOnly(1, &status);

			//			printf("Status: %d\t(status && 1)== %d\n",status,status&1);
//			printf("Tests: (5&&1)==%d    (4&&1)==%d  (36&&1)==%d\n",5&1, 4&1, 36&1);
		} while ((status & 1) == 1);

		std::uint8_t readRegister = LIDAR_DISTANCE_REGISTER;

		i2c.WriteBulk(&readRegister, 1);
		i2c.ReadOnly(2, distArray);

		measurement++;
	}//UpdateLidar


	void RobotInit(){

		printf("Not Calibrating\n");

		printf("Done\n");
	}

	void TeleopInit(){
		//Called once at the start of each operator period
		printf("TELEOP Initialized\n");

//		trigger.Set(false);
		distArray[0] = 255;
		distArray[1] = 254;

		printf("End of Teleop Init\n");
	}//Teleop Init
	void DisabledInit(){
		printf("Disabled TELEOP\n");
	}

	void TeleopPeriodic(){

//		printf("Loop\n");
		UpdateLidar();

		SmartDashboard::PutNumber("LidarDistance", (256*distArray[0]+distArray[1])/2.54);
		printf("\tDistance: %.0f inches\n", (256*distArray[0]+distArray[1])/2.54);

	}//teleop Periodic

	void TestPeriodic(){
	}

};

START_ROBOT_CLASS(LidarOnBoard)
