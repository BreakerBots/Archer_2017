/*
 * BreakerVision.h
 *
 *  Created on: Feb 15, 2017
 *      Author: DS_2016
 */

#ifndef SRC_BREAKERVISION_H_
#define SRC_BREAKERVISION_H_

class BreakerVision: public PIDSource, ITableListener{
public:
	typedef std::vector<double> PixyObjectData;

	std::shared_ptr<NetworkTable> pixyTable;
	//Todo: don't put magic constants in the class declaration.
	//Make them defaults that have set/get methods.
	int center_x;
	int center_y;
	const float distance_to_target_in_pixels = 244.462;

	bool trackObject;
	int objX;

	float error;

private:

	float target_width;
	float target_height;
	float epsilon;

	float target_y_lower;
	float target_y_higher;

	float target_distance_closer;
	float target_distance_farther;

public:

	BreakerVision ();
	void InitTable(std::shared_ptr<NetworkTable> table);

	void Update();

	//From PIDSource
	//Returns the offset of the targeted object(s) from
	//the center of the pixy's viewing pane.
	double PIDGet();

private:

	void FindTape (std::vector<PixyObjectData> &objects);
	void LoadObjectsFromPixy(std::vector<PixyObjectData> &objects);
	void EliminateMalformedObjects (std::vector<PixyObjectData> &objects);
	void EliminateMisproportionedObjects (std::vector<PixyObjectData> &objects);
	void EliminateHighObjects(std::vector<PixyObjectData> &objects);

	int GetTargetError(std::vector<PixyObjectData> &objects);

	void ValueChanged (ITable *source, llvm::StringRef key, std::shared_ptr< nt::Value > value, bool isNew);

};//Vision class

#endif /* SRC_BREAKERVISION_H_ */
