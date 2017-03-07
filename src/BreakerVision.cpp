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

BreakerVision::BreakerVision ():
		//Midpoint of the pixy cam's viewing window
	center_x(160),
	center_y(100),

		//If objects are spotted, track.
	trackObject(false),
		//Current tracking x
	objX(center_x),
		//error from current target
	error(0),

	target_width(3),
	target_height(5),
	epsilon(0.3),

	target_y_lower(90),
	target_y_higher(200),

	target_distance_closer(26),
	target_distance_farther(200)
{
} //Class Consructor

void BreakerVision::InitTable(std::shared_ptr<NetworkTable> table){
	//Inform user that slider 2 is in use
	pixyTable = table;
	pixyTable->AddTableListener(this);
	error = 0;

	if (
			pixyTable->ContainsKey("TargetData/target_width") &&
			pixyTable->ContainsKey("TargetData/target_height") &&
			pixyTable->ContainsKey("TargetData/epsilon") &&

			pixyTable->ContainsKey("TargetData/target_y_lower") &&
			pixyTable->ContainsKey("TargetData/target_y_higher") &&

			pixyTable->ContainsKey("TargetData/target_distance_closer") &&
			pixyTable->ContainsKey("TargetData/target_distance_farther")

	){

		target_width 	= pixyTable->GetNumber("TargetData/target_width",2);
		target_height 	= pixyTable->GetNumber("TargetData/target_height",5);
		epsilon 		= pixyTable->GetNumber("TargetData/epsilon",0.3);

		target_y_lower  = pixyTable->GetNumber("TargetData/target_y_lower",90);
		target_y_higher = pixyTable->GetNumber("TargetData/target_y_higher",200);

		target_distance_closer = pixyTable->GetNumber("TargetData/target_distance_closer",26);
		target_distance_farther = pixyTable->GetNumber("TargetData/target_distance_farther",200);

	} else {
		pixyTable->PutNumber("TargetData/target_width",2);
		pixyTable->PutNumber("TargetData/target_height",5);
		pixyTable->PutNumber("TargetData/epsilon",0.3);

		pixyTable->PutNumber("TargetData/target_y_lower",90);
		pixyTable->PutNumber("TargetData/target_y_higher",200);

		pixyTable->PutNumber("TargetData/target_distance_closer",26);
		pixyTable->PutNumber("TargetData/target_distance_farther",200);

	}

}//init method

void BreakerVision::Update(){
	/*
	 * Run continuously from Tele-Op
	 *
	 */
	std::vector<PixyObjectData> objects;

	FindTape(objects);
//	new_error = GetTargetError(objects);
	error = GetTargetError(objects);

//	lowpass_error =


	pixyTable->PutNumber("TargetData/error",error);

}//Loop method

double BreakerVision::PIDGet(){
	return error;
}//PIDGet method (from PIDSource)

void BreakerVision::FindTape(std::vector<PixyObjectData> &objects){

	LoadObjectsFromPixy(objects);
	EliminateMalformedObjects(objects);
//
	EliminateMisproportionedObjects(objects);
	EliminateHighObjects(objects);

}//FindTape

void BreakerVision::LoadObjectsFromPixy(std::vector<PixyObjectData> &objects){

	for (int i=0; i<pixyTable->GetNumber("Incoming/number_of_objects",0); i++){
		if (!pixyTable->ContainsKey("Incoming/object" + std::to_string(i))){
			printf("ERROR: BreakerVision: Incoming/object%d not found in Network Table\n",i);
			continue;
		}
		PixyObjectData object = pixyTable->GetNumberArray("Incoming/object" + std::to_string(i), PixyObjectData());
		objects.push_back(object);
	}
}//LoadObjectsFromPixy

void BreakerVision::EliminateMalformedObjects(std::vector<PixyObjectData> &objects){
	for (unsigned int i=0; i<objects.size(); i++){
		if (objects[i].size() != 5){
			objects.erase(objects.begin() + i);
		}
	}
}//EliminateMalformedObjects

void BreakerVision::EliminateMisproportionedObjects(std::vector<PixyObjectData> &objects){
	//Proportion bounds
	float lower_bound = (target_width/target_height)-epsilon;
	float upper_bound = (target_width/target_height)+epsilon;

	for (unsigned int i=0; i<objects.size(); i++){
		float width_to_height = objects[i][3] / objects[i][4];

		if (width_to_height < lower_bound || upper_bound < width_to_height)
			objects.erase(objects.begin() + i);
	}
}//EliminateMisproportionedObjects

void BreakerVision::EliminateHighObjects(std::vector<PixyObjectData> &objects){

	for (unsigned int i=0; i<objects.size(); i++){
		if (objects[i][1] < target_y_lower || target_y_higher < objects[i][1])
			objects.erase(objects.begin() + i);
	}

}//EliminateHighObjects

int BreakerVision::GetTargetError(std::vector<PixyObjectData> &objects){

	if (objects.size() == 0) {
		printf("Targetting on 0 objects!\n");
		pixyTable->PutNumber("TargetData/targetting_number",objects.size());
		pixyTable->PutNumber("TargetData/measured_distance",200);
		return error;
	}

//	if (objects.size() == 1) {
//		printf("Targetting on 1 object!\n");
//		pixyTable->PutNumber("TargetData/targetting_number",objects.size());
//		pixyTable->PutNumber("TargetData/measured_distance",200);
//	}

	int sum = 0;
	int sum_height = 0;

	for (unsigned int i=0;i<objects.size();i++){
		sum += objects[i][1];
		sum_height += objects[i][4];
	}

	printf("Targetting on %d objects!\n",objects.size());
	pixyTable->PutNumber("TargetData/targetting_number",objects.size());
	pixyTable->PutNumber("TargetData/measured_distance",244.462*5/ (sum_height/objects.size()) );

	if (objects.size() == 1) return error;

	return center_x - ( sum / objects.size() );

}//GetTargetError

void BreakerVision::ValueChanged (ITable *source, llvm::StringRef key, std::shared_ptr< nt::Value > value, bool isNew){
	if (	key.equals("TargetData/target_width") ||
			key.equals("TargetData/target_height") ||
			key.equals("TargetData/epsilon") ||

			key.equals("TargetData/target_y_lower") ||
			key.equals("TargetData/target_y_higher")
	){
		target_width = pixyTable->GetNumber("TargetData/target_width",3);
		target_height = pixyTable->GetNumber("TargetData/target_height",5);
		epsilon = pixyTable->GetNumber("TargetData/epsilon",0.05);

		target_y_lower  = pixyTable->GetNumber("TargetData/target_y_lower",90);
		target_y_higher = pixyTable->GetNumber("TargetData/target_y_higher",200);

		target_distance_closer = pixyTable->GetNumber("TargetData/target_distance_closer",26);
		target_distance_farther = pixyTable->GetNumber("TargetData/target_distance_farther",200);
	}
}//ValueChanged


