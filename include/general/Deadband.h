/*
 * Deadband.h
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */

#ifndef SRC_DEADBAND_H_
#define SRC_DEADBAND_H_

class Deadband {

private:
	float previousOutput;

	float deadbandMagnitude;
	float minY;
	float maxY;

	float mLeft;
	float mRight;
	float bLeft;
	float bRight;

public:

	Deadband (float deadband);
	Deadband (float deadband, float minimumYValue, float maximumYValue);

	float OutputFor (float input);
	float PreviousOutput ();

	void SetMinY (float newMinValue);
	float MinY ();

	void SetMaxY(float newMaxValue);
	float MaxY ();

	void SetDeadbandMagnitude (float newDeadbandMagnitude);
	float DeadbandMagnitude();

private:

	void SolveForLine ();
};

#endif /* SRC_DEADBAND_H_ */
