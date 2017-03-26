/*
 * PIDOutputEffort.h
 *
 *  Created on: Mar 18, 2017
 *      Author: DS-2017
 */

#ifndef PIDOUTPUTEFFORT_H_
#define PIDOUTPUTEFFORT_H_

#include "WPILib.h"

/*
 * PIDOutputEffort class allows for simple testing of PIDControllers. As the PIDController requires
 * a pointer to a PIDOutput object, this class fulfills that requirement while sending the output of
 * the PIDController to an easily viewable networktable (optional). The PIDOutputEffort also allows
 * for the use of a scalar function (default - Sigmoid).
 */

//Implement the PIDOutput class, for use in a PIDController
class PIDOutputEffort: public PIDOutput {

private:
	//A sub-networktable, to which the object will post raw_value and value
	std::shared_ptr<ITable> m_table;

	//Raw output from the PIDController
	double raw_value;
	//Output from PIDController scaled (-1 to 1)
	double value;

	//Scalar function
	double Scale_Effort_To_Arcade(double raw);

public:

	//Basic Constructor
	PIDOutputEffort ();

	//Returns raw_value
	double Raw_Value ();
	//Returns the scaled output
	double Value ();

	//Supply a subtable that the PIDOutputEffort can use
	void InitTable (std::shared_ptr<ITable> table);

	//Accept output from PIDController (Required from PIDOutput)
	void PIDWrite(double output);

};

#endif /* PIDOUTPUTEFFORT_H_ */
