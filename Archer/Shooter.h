/*
 * Shooter.h
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#ifndef SHOOTER_H_
#define SHOOTER_H_

#include "XBox.h"
#include "ToggleButton.h"

#include "Indexer.h"
#include "Spine.h"
#include "Cannon.h"

class Shooter {

public:
	enum Buttons {
		kShooterToggle = XBox::R3
	};

private:

	ToggleButton shooterButton;

	Indexer indexer;
	Spine spine;
	Cannon cannon;

	std::shared_ptr<ITable> table;

public:

	Shooter ();
	void InitTable (std::shared_ptr<ITable> table);

};

#endif /* SHOOTER_H_ */
