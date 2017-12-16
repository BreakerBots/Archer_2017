/*
 * PixyCam.h
 *
 *  Created on: Nov 30, 2017
 *      Author: BreakerBots
 */

#ifndef SRC_PIXYCAM_H_
#define SRC_PIXYCAM_H_

#include "I2C.h"
#include <chrono>
#include <thread>
#include <unistd.h>

#define PixyFrame std::vector<PixyObject>
#define START_BLOCK 43605

struct PixyObject {
	int checksum;
	int sig;
	int x, y;
	int w, h;
	PixyObject (){
		checksum = -1;
		sig = -1;
		x = 0, h = 0;
		w = 1, y = 1;
	}
};

class PixyCam {

public:
	static void startThread (PixyCam& pixy);

	PixyCam(int address);
	int frameCount();
	PixyFrame objects();

	void Start();

//private:
	//To be looped in a separate thread
	// Simply reads data from I2C bus
	void ReadData();

private:
	unsigned int m_addr;
	I2C *m_bus;
	std::thread *m_thread;

	bool m_running;
	std::string debug;
public:
	volatile int m_frame;
private:
	PixyFrame m_blocks;

};

#endif /* SRC_PIXYCAM_H_ */
