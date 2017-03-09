/*
 * Cannon.h
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#ifndef CANNON_H_
#define CANNON_H_

#include "WPILib.h"
#include "CANTalon.h"

#include "XBox.h"
#include "Talons.h"

#include "Deadband.h"

class Cannon : public ITableListener {

public:
	enum Buttons {
		kAngleAxis = XBox::RY
	};

private:

	Servo angleServo;
	int angle;

	CANTalon spinLeft;
	CANTalon spinRight;

	float speed;
	Deadband rxDeadband;
	Deadband ryDeadband;

	std::shared_ptr<ITable> table;

public:

	Cannon ();
	void InitTable (std::shared_ptr<ITable> table);

	void Update(Joystick &xbox, bool enabled);

	void ClearIaccum();

	void ValueChanged(ITable* source, llvm::StringRef key,
			std::shared_ptr<nt::Value> value, bool isNew);

};


#endif /* CANNON_H_ */
