/*
 * BreakerVision.cpp
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */

#include "WPILib.h"
#include "BreakerVision.h"

	//Todo: don't put magic constants in the class declaration.
	//Make them defaults that have set/get methods.

BreakerVision::BreakerVision (std::shared_ptr<NetworkTable> nt){
		//Midpoint of the pixy cam's viewing window
	center_x = 160;
	center_y = 100;
		//NetworkTable to which the pixy cam posts its values
	pixyTable = nt;
		//If objects are spotted by the pixy cam, track those objects
	trackObject = false;
		//# of objects to track targetX becomes the average
	tapeCount = 1;
		//Current targetX of the image
	objX = center_x;

	error = 0;
} //Class Consructor

void BreakerVision::Init(std::shared_ptr<NetworkTable> table){
	//Inform user that slider 2 is in use
	pixyTable = table;
	SmartDashboard::PutBoolean("DB/LED 2", true);
}//init method

void BreakerVision::Update(){
	/*
	 * Run continuously from Tele-Op
	 *
	 */

	tapeCount = SmartDashboard::GetNumber("DB/Slider 2",1);
	ScanForObjects();

}//Loop method

double BreakerVision::PIDGet(){
	return error;
}//PIDGet method (from PIDSource)

//Todo: Implement a try/catch pattern for reading Network tables...does
//      a Network Table throw if you try to read a value not in the table?
//		if not, then test for defaults before taking any further actions.
//Relies upon the following data being available on the Network Table
//	Frame
//	NumOfObjects
//	ObjectN for [0:N] objects
void BreakerVision::ScanForObjects(){
//	int frame = pixyTable->GetNumber("Frame",-1);
	int trackedObjectCount = pixyTable->GetNumber("NumOfObjects",0);

	if (trackedObjectCount > 0){

		//Average X values of objects
		float x_sum = 0;
		int count = 0;

		for (int i=0; i<std::min(tapeCount,trackedObjectCount); i++){
			std::vector<double> object = pixyTable->GetNumberArray("Object"+std::to_string(i),std::vector<double>());
			x_sum += object[1];
			count++;
		}

		objX = x_sum/count;

		error = center_x-objX;

		trackObject = true;
	} else {
		trackObject = false;
		error = 0;
	}
	pixyTable->PutNumber("Error",error);

}//ScanForObjects method



