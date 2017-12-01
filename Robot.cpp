#include <WPILib.h>
#include <IterativeRobot.h>

#include "I2C.h"
#include <chrono>
#include <unistd.h>

#define PIXY_ADDR 0x54

class Robot: public frc::IterativeRobot {
private:
	I2C pixy, falsePixy;
	std::string s;


public:
	Robot():
		pixy(I2C::Port::kMXP, PIXY_ADDR),
		falsePixy(I2C::Port::kMXP, 0x55)
	{
		s = "";
	}

	void RobotInit() {
	}

	void AutonomousInit() override {
	}

	void AutonomousPeriodic() {
	}

	void TeleopInit() {
	}

	void TeleopPeriodic() {
		s = "Teleop: ";

		unsigned char data[2];

		while (IsEnabled()){
			pixy.ReadOnly(2, data);
			int block = (unsigned int)(data[1])*256+(unsigned int)(data[0]);
			s += "\n"+std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
			s += "\t"+std::to_string(block);
			if (block == 0)
				usleep(1000*20);
		}
		std:: cout << s << std::endl;


//		std::cout << std::hex << (unsigned int)(data[1])*256+(unsigned int)(data[0]) << std::endl;
	}

	void DisabledInit() {
	}

	void TestPeriodic() {
	}

};

START_ROBOT_CLASS(Robot)
