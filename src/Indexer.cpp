/*
 * Indexer.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#include "WPILib.h"
#include "CANTalon.h"

#include "Talons.h"

#include "Indexer.h"

Indexer::Indexer ():
	talon (Talons::INDEXER),
	speed (100)
{
}//Indexer

void Indexer::InitTable(std::shared_ptr<ITable> subtable){

	table = subtable;

	table->PutNumber("speed",speed);

	table->PutNumber("p",talon.GetP());
	table->PutNumber("i",talon.GetI());
	table->PutNumber("d",talon.GetD());
	table->PutNumber("f",talon.GetF());

	table->PutNumber("izone",talon.GetIzone());
	table->PutBoolean("clearintegral",false);

}//InitTable

void Indexer::Update (bool enabled){

	if (enabled){
		talon.Set(speed);
	} else {
		talon.Set(0);
	}

}//Update

void Indexer::ValueChanged(ITable *source, llvm::StringRef key, std::shared_ptr<nt::Value> value, bool isNew){

	if (	key == "speed" ||
			key == "p" || key == "i" || key == "d" || key == "f" ||
			key == "izone"){

		speed = table->GetNumber("speed",speed);
		talon.SetP		(table->GetNumber("p",talon.GetP()));
		talon.SetI		(table->GetNumber("i",talon.GetI()));
		talon.SetD		(table->GetNumber("d",talon.GetD()));
		talon.SetF		(table->GetNumber("f",talon.GetF()));
		talon.SetIzone	(table->GetNumber("izone",talon.GetIzone()));

	} else if (key == "clearintegral"){
		if (table->GetBoolean("clearintegral",false)){
			talon.ClearIaccum();
			table->PutBoolean("clearintegral",false);
		}
	}

}//ValueChanged


