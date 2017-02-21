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

BreakerVision::BreakerVision (){
		//Midpoint of the pixy cam's viewing window
	center_x = 160;
	center_y = 100;
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

//Relies upon the following data being available on the Network Table
//	Frame
//	NumOfObjects
//	ObjectN for [0:N] objects
void BreakerVision::ScanForObjects(){
	if (!pixyTable->ContainsKey("NumOfObjects")){
		error = 0;

//		printf("PixyTable does not contain key: NumOfObjects\n");
		return;
	}

	int trackedObjectCount = pixyTable->GetNumber("NumOfObjects",0);
/*
	if (trackedObjectCount > 0){

		//Average X values of objects
		float x_sum = 0;
		int count = 0;


		for (int i=0; i<std::min(tapeCount,trackedObjectCount); i++){
			if (!pixyTable->ContainsKey("Object"+std::to_string(i))){
				printf("ERROR: BreakerVision: Object%d not found in Network Table\n",i);
				continue;
			}
			std::vector<double> object = pixyTable->GetNumberArray("Object"+std::to_string(i),std::vector<double>());

			if (object.size() < 5){
				printf("ERROR: BreakerVision: Object%d has <5 elements\n",i);
				continue;
			}
			x_sum += object[1];
			count++;
		}

		int targetHeight = -1;//Height of target object
		std::vector<int> heights = std::vector<int>();
		printf("\n\nNewLoop\n");
		for (int i=0; i<trackedObjectCount; i++){
			printf("Reading Object %d\n",i);
			if (!pixyTable->ContainsKey("Object"+std::to_string(i))){
				printf("ERROR: BreakerVision: Object%d not found in Network Table\n",i);
				continue;
			}
			std::vector<double> object = pixyTable->GetNumberArray("Object"+std::to_string(i),std::vector<double>());
			if (0.3 < object[3]/object[4] && object[3]/object[4]<0.7){
				if (object.size() < 5){
					printf("ERROR: BreakerVision: Object%d has <5 elements\n",i);
					continue;
				}
				printf("Lift Target?: ");
				for (int num : object){
					printf("%4d",num);
				}
				printf("\n");
				heights.push_back(object[4]);
			}
		}

		printf("Heights: %d\n",heights.size());

		if (count == 0){
			printf("ERROR: BreakerVision: Insufficient # of objects posted to Network Table\n");
			error = 0;
			trackObject = false;
			return;
		}
		objX = x_sum/count;

		error = center_x-objX;

		trackObject = true;

		if (targetHeight == 0){
			pixyTable->PutNumber("Distance to Tape",-1);
		} else {
			pixyTable->PutNumber("Distance to Tape",244.462*5/targetHeight);
		}

	} else {
		trackObject = false;
		error = 0;
		pixyTable->PutNumber("Distance to Tape",-1);
	}
	pixyTable->PutNumber("Error",error);
*/
}//ScanForObjects method

