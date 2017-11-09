/*
 * TalonEncoder.h
 *
 *  Created on: Mar 1, 2017
 *      Author: DS-2017
 */

#ifndef TALONENCODER_H_
#define TALONENCODER_H_

#include "WPILib.h"
#include "ctrlib/CANTalon.h"

class TalonEncoder : public PIDSource {
private:
	CANTalon &talon;

public:
	TalonEncoder (CANTalon &readTalon);

	double PIDGet();
};

#endif /* TALONENCODER_H_ */
