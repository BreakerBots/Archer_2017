#include <IterativeRobot.h>

#include "I2C.h"
#include <chrono>
#include <unistd.h>
#include <thread>

#include "PixyCam.h"
#define PIXY_ADDR 0x54

class Robot: public frc::IterativeRobot {
private:

	PixyCam pixy;
//	I2C bus;

public:
	Robot():
		pixy(PIXY_ADDR)
//		bus(I2C::Port::kMXP, 0x54)
	{
	}

	void TeleopInit() {
		pixy.Start();
	}

	void TeleopPeriodic() {
/*
		s = "Teleop: ";

*/
		std::cout << "PixyFrame: " << pixy.frameCount()
				  << "  Object Count: " << pixy.objects().size();
		if (pixy.objects().size())
			std::cout << "  Object Height: " << pixy.objects().front().h;
		std::cout << std::endl;

//		pixy.ReadData();
//		unsigned char data[2];
//
//		while (IsEnabled()){
//			bus.ReadOnly(2, data);
//			int block = (unsigned int)(data[1])*256+(unsigned int)(data[0]);
//			std::string s = "Teleop: ";
//			s += "\n"+std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
//			s += "\t"+std::to_string(block);
//			std::cout << s << std::endl;
//			if (block == 0)
//				usleep(1000*20);
//		}
//		std:: cout << s << std::endl;
//		std::cout << "Pixy Frame #: " << pixy.frameCount() << std::endl;

	}//TeleopPeriodic

};

START_ROBOT_CLASS(Robot)
