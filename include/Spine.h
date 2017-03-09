/*
 * Spine.h
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#ifndef SPINE_H_
#define SPINE_H_

#include "WPILib.h"
#include "CANTalon.h"

class Spine : public ITableListener{

private:

	CANTalon talon;
	float speed;

	std::shared_ptr<ITable> table;

public:

	Spine ();
	void InitTable (std::shared_ptr<ITable> table);

	void Update(bool enabled);

	void ValueChanged(ITable* source, llvm::StringRef key,
			std::shared_ptr<nt::Value> value, bool isNew);
};

#endif /* SPINE_H_ */
