/*
 * Cannon.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: DS-2017
 */

#include "WPILib.h"
#include "CANTalon.h"

#include "Talons.h"

#include "Cannon.h"
#include "VelocityController.h"

Cannon::Cannon ():

	angleServo(0),
	angle(0/*Starting angle*/),

	spinLeft(Talons::kLeft),
	spinRight(Talons::kRight),

	speed(200),
	rxDeadband(0.1),
	ryDeadband(0.1)

{
	spinLeft.SetControlMode(CANTalon::kSpeed);
	spinLeft.SetFeedbackDevice(CANTalon::QuadEncoder);
	spinLeft.SetPID(0,0,0,0);

	spinRight.SetControlMode(CANTalon::kSpeed);
	spinRight.SetFeedbackDevice(CANTalon::QuadEncoder);
	spinRight.SetPID(0,0,0,0);

}//Cannon

void Cannon::InitTable (std::shared_ptr<ITable> itable){

	table = itable;

	table->PutNumber("angle",angle);
	table->PutNumber("speed",speed);

	table->PutNumber("PID/p",spinLeft.GetP());
	table->PutNumber("PID/i",spinLeft.GetI());
	table->PutNumber("PID/d",spinLeft.GetD());
	table->PutNumber("PID/f",spinLeft.GetF());
	table->PutNumber("PID/izone",spinLeft.GetIzone());
	table->PutNumber("PID/clearintegral",false);

}//InitTable

void Cannon::Update (Joystick &xbox, bool enabled){

	if (enabled){

		//Temporarily, use RX to speed up/down the cannon
		speed += 10*rxDeadband.OutputFor(xbox.GetRawAxis(XBox::RX));
		angle += 10*ryDeadband.OutputFor(xbox.GetRawAxis(Buttons::kAngleAxis));

		angleServo.Set(angle);
		spinLeft.Set(speed);
		spinRight.Set(speed);

		if (table){
			table->PutNumber("pov",xbox.GetPOV(0));
			table->PutNumber("speed",speed);
			table->PutNumber("angle",angle);
		}

	} else {
		spinLeft.Set(0);
		spinRight.Set(0);
	}

}//Update

void Cannon::ClearIaccum(){

	spinLeft.ClearIaccum();
	spinRight.ClearIaccum();

}

void Cannon::ValueChanged(ITable *source, llvm::StringRef key, std::shared_ptr<nt::Value> value, bool isNew){
	if (	key == "angle" || key == "speed" ||
			key == "p" || key == "i" || key == "d" || key == "f" ||
			key == "izone"){
		angle = table->GetNumber("angle",angle);
		speed = table->GetNumber("speed",speed);

		spinLeft.SetP(table->GetNumber("PID/p",spinLeft.GetP()));
		spinLeft.SetI(table->GetNumber("PID/i",spinLeft.GetI()));
		spinLeft.SetD(table->GetNumber("PID/d",spinLeft.GetD()));
		spinLeft.SetF(table->GetNumber("PID/f",spinLeft.GetF()));
		spinLeft.SetIzone(table->GetNumber("PID/izone",spinLeft.GetIzone()));

		spinRight.SetP(table->GetNumber("PID/p",spinRight.GetP()));
		spinRight.SetI(table->GetNumber("PID/i",spinRight.GetI()));
		spinRight.SetD(table->GetNumber("PID/d",spinRight.GetD()));
		spinRight.SetF(table->GetNumber("PID/f",spinRight.GetF()));
		spinLeft.SetIzone(table->GetNumber("PID/izone",spinLeft.GetIzone()));

	} else if (key == "clearintegral"){

		if (table->GetNumber("clearintegral",false)){
			spinLeft.ClearIaccum();
			spinRight.ClearIaccum();

			table->PutNumber("clearintegral",false);
		}
	}


}//ValueChanged

