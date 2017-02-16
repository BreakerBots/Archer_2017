
#ifndef WINGS_H
#define WING_H

#include "WPILib.h"
#include "ToggleButton.h"

class Wings {

private:
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

};

#endif
