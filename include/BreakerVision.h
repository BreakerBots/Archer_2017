/*
 * BreakerVision.h
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */

#ifndef SRC_BREAKERVISION_H_
#define SRC_BREAKERVISION_H_

class BreakerVision: public PIDSource{
public:

	std::shared_ptr<NetworkTable> pixyTable;
	//Todo: don't put magic constants in the class declaration.
	//Make them defaults that have set/get methods.
	int center_x;
	int center_y;

	bool trackObject;
	int tapeCount;
	int objX;

	float error;

	BreakerVision (std::shared_ptr<NetworkTable> nt);
	void Init(std::shared_ptr<NetworkTable> table);

	void Update();

	//From PIDSource
	//Returns the offset of the targeted object(s) from
	//the center of the pixy's viewing pane.
	double PIDGet();

private:

	//Todo: Implement a try/catch pattern for reading Network tables...does
	//      a Network Table throw if you try to read a value not in the table?
	//		if not, then test for defaults before taking any further actions.
	//Relies upon the following data being available on the Network Table
	//	Frame
	//	NumOfObjects
	//	ObjectN for [0:N] objects
	void ScanForObjects();


};//Vision class

#endif /* SRC_BREAKERVISION_H_ */
