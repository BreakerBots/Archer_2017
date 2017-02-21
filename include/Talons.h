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
		TOP=27, BOTTOM=28, TILT=29, PIVOT=30
		//Archer
	};

	enum Winch {
		WINCH=8
	};
	//Not sure about talon count / name
	enum Slurper {
		INTAKE=7
	};

	enum Spine {
		INDEXER=12
	};
};



#endif /* SRC_TALONS_H_ */
