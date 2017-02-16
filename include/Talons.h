/*
 * Talons.h
 *
 *  Created on: Feb 8, 2017
 *      Author: DS_2016
 */

#ifndef SRC_TALONS_H_
#define SRC_TALONS_H_

class Talons {
public:
	enum DriveSystem {
		//Chadwick
		// 1   2   3   4   5   6
		R1=1, L1=2, R2=3, L2=4, R3=5, L3=6
		//Archer

	};

	enum Shooter {
		//Chadwick
		//Not Checked / Made
		TOP=7, BOTTOM=8, TILT=9, PIVOT=10
		//Archer
	};

	enum Winch {
		WINCH=40
	};
	//Not sure about talon count / name
	enum Slurper {
		INTAKE=11
	};

	enum Spine {
		INDEXER=12
	};
};



#endif /* SRC_TALONS_H_ */
