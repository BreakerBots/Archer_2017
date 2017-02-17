/*
 * MyPIDController.h
 *
 *  Created on: Feb 17, 2017
 *      Author: DS_2016
 */

#ifndef MYPIDCONTROLLER_H_
#define MYPIDCONTROLLER_H_

#include "WPILib.h"

class MyPIDController: public PIDController {

public:
	static const int IZone = 60;

	MyPIDController (double p, double i, double d, PIDSource *source, PIDOutput *output);

//	void Calculate();
};

#endif /* MYPIDCONTROLLER_H_ */
