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
	int checksum,
		sig,
		x, y,
		w, h;
};

class PixyCam {



private:
	void run(){

	}

private:
	unsigned int m_addr;
	I2C m_bus;


};

#endif /* SRC_PIXYCAM_H_ */
