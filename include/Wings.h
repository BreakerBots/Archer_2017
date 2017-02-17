
#ifndef WINGS_H
#define WING_H

#include "WPILib.h"
#include "ToggleButton.h"

class Wings {

	/*
	 * The Wings class manages the opening/closing of
	 * the gear holder on the robot (Chadwick/Archer).
	 * Currently, the X Button on the xbox controller
	 * toggles the state of the wings.
	 *
	 * As a debug/monitoring device, several values are
	 * posted to the RoboRIOs Network Table. For simplicity
	 * and safety, the Wings classes only has access to
	 * the Subsystems/Wings SubTable. It writes the fol-
	 * lowing values:
	 * 		-	PCM_ID			-- ID of the PCM
	 * 		-	State			-- OPEN or CLOSED
	 * 	These next two values output DoubleSolenoid::Value
	 * 	0 == kOff	1 == kForward	2 == kReverse
	 * 		-	Debug/RightWing	-- 1, or 2
	 * 		-	Debug/LeftWing	-- 1, or 2
	 */

private:

	std::shared_ptr<ITable> wingsTable;

	const int PCM_ID = 50;

	DoubleSolenoid* leftWing;
	bool leftForward;
	//leftBack = !leftForward

	DoubleSolenoid* rightWing;
	bool rightForward;
	//rightBack = !rightForward
	ToggleButton button;

public:

	Wings ();
	Wings (int trigger);

	void Init (std::shared_ptr<ITable> nt);
	void Update(const Joystick& xbox);

	void SetLeftForward(bool newLeftForward);
	bool GetLeftForward();
	void SetRightForward(bool newRightForward);
	bool GetRightForward();

	void SetTriggerButton(int button);
	int GetTriggerButton();

	void Open();
	void Close();
	bool AreClosed();

	void PostValues();

};

#endif
