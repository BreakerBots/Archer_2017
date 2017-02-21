/*
 * Deadband.cpp
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */

#include "Deadband.h"


Deadband::Deadband (float deadband){
	deadbandMagnitude = deadband;
	minY = -1;
	maxY = 1;

	SolveForLine();
}
Deadband::Deadband (float deadband, float lowest, float highest){
	deadbandMagnitude = deadband;
	minY = lowest;
	maxY = highest;

	SolveForLine();
}//Specific Constructor

float Deadband::OutputFor (float input){
	//Basic Slope-Intercept Formula
	float output;
	if (input < -deadbandMagnitude){
		output = mLeft*input+bLeft;
	} else if (input > deadbandMagnitude){
		output = mRight*input+bRight;
	} else {
		output = 0;
	}
	previousOutput = output;
	return output;
}//OutputFor method
float Deadband::PreviousOutput(){
	return previousOutput;
}

void Deadband::SetMinY (float newMinValue){
	minY = newMinValue;
	SolveForLine();
}
float Deadband::MinY (){
	return minY;
}

void Deadband::SetMaxY (float newMaxValue){
	maxY = newMaxValue;
	SolveForLine();
}
float Deadband::MaxY (){
	return maxY;
}

void Deadband::SetDeadbandMagnitude (float newDeadbandMagnitude){
	deadbandMagnitude = newDeadbandMagnitude;
	SolveForLine();
}
float Deadband::DeadbandMagnitude(){
	return deadbandMagnitude;
}

void Deadband::SolveForLine(){
	//Solve for two lines

	//(-1,minY) to (-deadband, 0)

	mLeft = (0-minY)/(1-deadbandMagnitude);
	bLeft = mLeft+minY;

	//(deadband,0) to (1,maxY)
	mRight = (maxY-0)/(1-deadbandMagnitude);
	bRight = maxY-mRight;

}//SolveForLine method


