/*
 * TalonEncoder.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: DS-2017
 */

#include "WPILib.h"
#include "TalonEncoder.h"


TalonEncoder::TalonEncoder(TalonSRX &readTalon):
	talon(readTalon)
{
}

double TalonEncoder::PIDGet(){
	return talon.GetSelectedSensorPosition(0);
}
