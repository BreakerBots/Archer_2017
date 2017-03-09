/*
 * Indexer.cpp
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#include "WPILib.h"
#include "CANTalon.h"

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

void Indexer::ValueChanged(ITable *source, llvm::StringRef key, std::shared_ptr<nt::Value> value, bool isNew){

	if (key == "speed" || key == "p" || key == "i" || key == "d" || key == "f" ||
			key == "izone" || key == "clearintegral"){

		speed = table->GetNumber("speed",speed);


	}

}//ValueChanged


