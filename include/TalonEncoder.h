/*
 * TalonEncoder.h
 *
 *  Created on: Mar 1, 2017
 *      Author: DS-2017
 */

#ifndef TALONENCODER_H_
#define TALONENCODER_H_

#include <PIDSource.h>
#include <ctre/Phoenix.h>

class TalonEncoder : public PIDSource {
private:
	TalonSRX &talon;

public:
	TalonEncoder (TalonSRX &readTalon);

	double PIDGet();
};

#endif /* TALONENCODER_H_ */
