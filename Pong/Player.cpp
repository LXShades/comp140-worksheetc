#include "Player.h"
#include <assert.h>
#include <windows.h>

#include "ArduinoController.h"

void Player::init()
{
	w = 10;
	h = 60;

	posX = 0;
	posY = 220;

	speed = 10;
}

void Player::Move()
{
	assert(mySide != Side_Undefined);
	assert(controller != nullptr);

#ifdef REGULAR_BORING_KEYBOARD_CONTROLS
	float delta = 0;

	if(mySide == Side_Left)
	{		
		if(GetAsyncKeyState('Q') != 0)
		{
			delta -= speed;
		}

		if(GetAsyncKeyState('A') != 0)
		{
			delta += speed;
		}
	}

	if(mySide == Side_Right)
	{
		if(GetAsyncKeyState('P') != 0)
		{
			delta -= speed;
		}

		if(GetAsyncKeyState('L') != 0)
		{
			delta += speed;
		}
	}

	if(( posY + delta > 100) && ((posY + delta) < (600 - h )))
	{
		posY += delta;
	}
#endif

	if (mySide == Side_Left)
	{
		posY = KnobToY(controller->GetLeftKnob());
	}
	else
	{
		posY = KnobToY(controller->GetRightKnob());
	}
}

void Player::SetSide(Side eSide)
{
	mySide = eSide;

	if(mySide == Side_Left)
	{
		posX = 30;
	}

	if(mySide == Side_Right)
	{
		posX = 760;
	}
}

void Player::SetController(const ArduinoController* controller_)
{
	this->controller = controller_;
}