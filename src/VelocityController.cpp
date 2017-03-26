/*
 * VelocityController.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: DS-2017
 */

#include "WPILib.h"
#include "CANTalon.h"

#include "VelocityController.h"

VelocityController::VelocityController (int talon_id):
	talon(talon_id)
{
	Initialize(0, 0, 0, 0, 0);

}//VelocityController

VelocityController::VelocityController(int talon_id, float p, float i, float d, float f, float izone):
	talon(talon_id)
{
	Initialize(p, i, d, f, izone);
}

void VelocityController::Initialize(float pGain, float iGain, float dGain, float feedForward, float izone){

	talon.SetControlMode(CANTalon::kSpeed);
	talon.Set(0);

	talon.SetPID(pGain, iGain, dGain, feedForward);

	if (izone >= 0)
		talon.SetIzone(izone);

}//Initialize

void VelocityController::InitTable(std::shared_ptr<ITable> subtable){

	table = subtable;

	table->PutNumber("speed",speed);

	table->PutNumber("p",talon.GetP());
	table->PutNumber("i",talon.GetI());
	table->PutNumber("d",talon.GetD());
	table->PutNumber("f",talon.GetF());

	table->PutNumber("izone",talon.GetIzone());
	table->PutBoolean("clearintegral",false);

}//InitTable

void VelocityController::Update (bool enabled){

	if (enabled){
		talon.Set(speed);
	} else {
		talon.Set(0);
	}

}//Update


//--------------------Setters and Getters-----------------//

void VelocityController::SetSpeed(float newSpeed){
	speed = newSpeed;

	if (table)
		table->PutNumber("speed",speed);

}//SetSpeed

float VelocityController::Speed(){
	return speed;
}//Speed

void VelocityController::SetPID (float p, float i, float d, float f){
	talon.SetPID(p, i, d, f);

	if (table){
		table->PutNumber("p",talon.GetP());
		table->PutNumber("i",talon.GetI());
		table->PutNumber("d",talon.GetD());
		table->PutNumber("f",talon.GetF());
	}
}

void VelocityController::SetP (float p){
	talon.SetP(p);

	if (table)
		table->PutNumber("p",talon.GetP());
}
float VelocityController::GetP (){
	return talon.GetP();
}
void VelocityController::SetI (float i){
	talon.SetI(i);

	if (table)
		table->PutNumber("i",talon.GetI());
}
float VelocityController::GetI (){
	return talon.GetI();
}
void VelocityController::SetD (float d){
	talon.SetD(d);

	if (table)
		table->PutNumber("d",talon.GetD());
}
float VelocityController::GetD (){
	return talon.GetD();
}
void VelocityController::SetF (float f){
	talon.SetF(f);

	if (table)
		table->PutNumber("f",talon.GetF());
}
float VelocityController::GetF (){
	return talon.GetF();
}

void VelocityController::SetIzone (float izone){
	talon.SetIzone(izone);

	if (table)
		table->PutNumber("izone",izone);

}
float VelocityController::GetIzone(){
	return talon.GetIzone();
}
void VelocityController::ClearIaccum(){
	talon.ClearIaccum();
}//ClearIaccum

void VelocityController::ValueChanged(ITable *source, llvm::StringRef key, std::shared_ptr<nt::Value> value, bool isNew){

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
