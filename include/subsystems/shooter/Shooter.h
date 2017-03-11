
/*
 * Shooter.h
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#ifndef SHOOTER_H_
#define SHOOTER_H_

#include "WPILib.h"

#include "general/XBox.h"
#include "general/ToggleButton.h"

#include "VelocityController.h"

#include "Cannon.h"

class Shooter {

public:
	enum Buttons {
		kShooterToggle = XBox::R3
	};

private:

	ToggleButton enableButton;

	VelocityController indexer;
	VelocityController spine;
	Cannon cannon;

	std::shared_ptr<ITable> table;

public:

	Shooter ();

	void Update (Joystick &xbox);

	void InitTable (std::shared_ptr<ITable> table);

	void ValueChanged(ITable* source, llvm::StringRef key,
				std::shared_ptr<nt::Value> value, bool isNew);

};

#endif /* SHOOTER_H_ */
