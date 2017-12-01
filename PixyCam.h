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
#include <unistd.h>

#define PixyFrame std::vector<PixyObject>

struct PixyObject {
	int checksum;
	int sig,
	int x, y,
	int w, h;
	PixyObject (){
		checksum = -1;
		sig = -1;
		x = 0, y = 0;
		w = 1, y = 1;
	}
};


class PixyCam {

public:
	PixyCam(unsigned int pixycam_address);

private:
	//To be looped in a separate thread
	// Simply reads data from I2C bus
	void ReadData();

private:
	unsigned int m_addr;
	I2C m_bus;

	bool m_running;
	int m_frame;
	PixyFrame m_blocks;


};

#endif /* SRC_PIXYCAM_H_ */
