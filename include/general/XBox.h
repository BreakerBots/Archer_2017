/*
 * XBox.h
 *
 *  Created on: Feb 8, 2017
 *      Author: DS_2016
 */

#ifndef SRC_XBOX_H_
#define SRC_XBOX_H_

class XBox {
public:
	enum Buttons {
		//1  2  3  4   5   6     7      8   9  10
		A=1, B, X, Y, LB, RB, BACK, START, L3, R3
	};
	enum Axis {
		// 0   1   2   3   4   5
		LX=0, LY, LT, RT, RX, RY
	};
};

#endif /* SRC_XBOX_H_ */
