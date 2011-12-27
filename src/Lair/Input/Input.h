#pragma once

class InputMan
{
private:
	typedef struct
	{
		bool	bState;
		float	fTimeStamp;	// time when it changed status
	
	} MouseButtonState;

public:
	InputMan()
	{
		mMouseButtonState[0].bState = false;
		mMouseButtonState[1].bState = false;
		mMouseButtonState[2].bState = false;
	}

	void UpdateMouseButtonState( int inButtonIndex, bool inState )
	{
		if( mMouseButtonState[inButtonIndex].bState = inState )
			mMouseButtonState[inButtonIndex].fTimeStamp = 0.0f; //todo

		mMouseButtonState[inButtonIndex].bState = inState;
	}

	const MouseButtonState& GetMouseButtonState( int inButtonIndex ) { return mMouseButtonState[inButtonIndex]; }

private:
	MouseButtonState	mMouseButtonState[3];	// support only 3 buttons
};