/*
 * VelocityControl.h
 *
 *  Created on: Mar 9, 2017
 *      Author: DS-2017
 */

#ifndef VELOCITYCONTROLLER_H_
#define VELOCITYCONTROLLER_H_

#include "WPILib.h"
#include "CANTalon.h"

class VelocityController : public ITableListener{

private:

	CANTalon talon;
	float speed;

	std::shared_ptr<ITable> table;

	void Initialize (float pGain, float iGain, float dGain, float feedForward, float izone);

public:

	VelocityController(int talon_id);
	VelocityController(int talon_id, float p, float i, float d, float f, float izone);

	void InitTable (std::shared_ptr<ITable> table);

	void Update(bool enabled);

	void SetSpeed (float newSpeed);
	float Speed ();

	void SetPID (float p, float i, float d, float f);
	void SetP (float p);
	float GetP ();
	void SetI (float i);
	float GetI ();
	void SetD (float d);
	float GetD ();
	void SetF (float f);
	float GetF ();

	void SetIzone (float izone);
	float GetIzone();
	void ClearIaccum();

	void ValueChanged(ITable* source, llvm::StringRef key,
			std::shared_ptr<nt::Value> value, bool isNew);

};

#endif /* VELOCITYCONTROLLER_H_ */
