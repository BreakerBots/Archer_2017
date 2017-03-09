/*
 * Shooter.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: DS-2017
 */

#include "WPILib.h"

#include "XBox.h"
#include "ToggleButton.h"
#include "VelocityController.h"

#include "Shooter.h"
/*
	ToggleButton shooterButton;

	VelocityController indexer;
	VelocityController spine;
	Cannon cannon;

	std::shared_ptr<ITable> table;
*/

Shooter::Shooter ():
	enableButton(Buttons::kShooterToggle),

	indexer(Talons::INDEXER),
	spine(Talons::SPINE),
	cannon()

{
}

void Shooter::Update (Joystick &xbox){

	enableButton.Update(xbox);

	if (enableButton.State() && enableButton.PrevState()){
		indexer.ClearIaccum();
		spine.ClearIaccum();
		cannon.ClearIaccum();
	}

	indexer.Update(enableButton.State());
	spine.Update(enableButton.State());
	cannon.Update(xbox, enableButton.State());

}//Update

void Shooter::InitTable (std::shared_ptr<ITable> itable){

	table = itable;

	indexer.InitTable(itable);
	spine.InitTable(itable);
	cannon.InitTable(itable);

	table->PutBoolean("enabled",enableButton.State());

}//InitTable

void Shooter::ValueChanged(ITable* source, llvm::StringRef key,
			std::shared_ptr<nt::Value> value, bool isNew){

	if (key == "enabled"){

		enableButton.Override(table->GetBoolean("enabled",enableButton.State()));

	}

}//ValueChanged

