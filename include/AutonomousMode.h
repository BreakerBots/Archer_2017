/*
 * AutonomousMode.h
 *
 *  Created on: Mar 8, 2017
 *      Author: DS-2017
 */

#ifndef AUTONOMOUSMODE_H_
#define AUTONOMOUSMODE_H_

class AutonomousMode {

public:
	virtual ~AutonomousMode() = default;

	virtual void Init() = 0;
	virtual void Periodic() = 0;
	virtual void Done() = 0;

};

#endif /* AUTONOMOUSMODE_H_ */
