/*
 * PIDOutputEffort.cpp
 *
 *  Created on: Mar 18, 2017
 *      Author: DS-2017
 */

#include "PIDOutputEffort.h"

PIDOutputEffort::PIDOutputEffort(){
	//Initialize values to 0
	raw_value = 0;
	value = 0;
}//Constructor

//'Getters' for the raw/scaled values
double PIDOutputEffort::Raw_Value(){
	return raw_value;
}//raw_value
double PIDOutputEffort::Value(){
	return value;
}//Value


void PIDOutputEffort::InitTable(std::shared_ptr<ITable> table){
	//Store ptr to subtable
	m_table = table;

	//Put current values to table
	m_table->PutNumber("raw_value",raw_value);
//	m_table->PutNumber("value",value);
}//InitTable

void PIDOutputEffort::PIDWrite(double output){
	//Store output from PIDController
	raw_value = output;

	//Scale raw_value through sigmoid function
	value = Scale_Effort_To_Arcade(raw_value);

	//Post values to networktable
	if (m_table){
		m_table->PutNumber("raw_value",raw_value);
//		m_table->PutNumber("value",value);
	}
}//PIDWrite

double PIDOutputEffort::Scale_Effort_To_Arcade(double raw){
	return 2/(1+exp(-raw)) - 1;
}

