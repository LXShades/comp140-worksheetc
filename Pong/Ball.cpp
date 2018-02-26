#include "Ball.h"
#include "ArduinoController.h"

void Ball::init()
{
	w = h = 10;
	posX = 400;posY = 300;

	ServeTo(Player::Side_Left);
}

void Ball::Move()
{	
	if(((posY + dy) < 100) || ((posY +dy) > 590))
	{
		dy = -dy;
	}

	posX += dx;
	posY += dy;
}

void Ball::SetController(ArduinoController* Controller_)
{
	this->Controller = Controller_;
}

void Ball::PlayerReturns()
{
	// Flash lights to indiate a hit
	if (dx < 0.0f)
	{
		Controller->FlashLeftLEDs(1);
	}
	else
	{
		Controller->FlashRightLEDs(1);
	}

	// 
	dx = -dx;
	posX += dx;
	posY += dy;

	// Accelerate
	const float AccelerationPerHit = 0.5f;
	const float CurrentSpeed = sqrt(dx * dx + dy * dy);

	dx *= (CurrentSpeed + AccelerationPerHit) / CurrentSpeed;
	dy *= (CurrentSpeed + AccelerationPerHit) / CurrentSpeed;

	// Predict where it'll land on the other side
	UpdateDestiny();
}

void Ball::UpdateDestiny()
{
	const int LeftPaddleX = 30 + 10, RightPaddleX = 760 - 10;
	int TargetY = 0;
	//numDebugPoints = 0;
	float SimDx = dx, SimDy = dy;
	float SimX = posX, SimY = posY;
	int NextLeftY = -1, NextRightY = -1;
	bool NextLeftFound = false, NextRightFound = false;
	for (int iterations = 0; iterations < 10 && (!NextLeftFound || !NextRightFound); iterations++) {
		int NextBounceX;
		int NextBounceY;

		// Try simulating movement to the next vertical bounce
		if (SimDy < 0)
		{
			NextBounceX = SimX + (100 - SimY) * SimDx / SimDy;
			NextBounceY = 100;
		}
		else
		{ 
			NextBounceX = SimX + (590 - SimY) * SimDx / SimDy;
			NextBounceY = 590;
		}

		// Check if this was beyond the paddle, and if so, find the point where they'd meet
		if (NextBounceX < LeftPaddleX)
		{
			NextLeftY = SimY + (LeftPaddleX - SimX) * SimDy / SimDx;
			SimDx = -SimDx;
			SimX = LeftPaddleX;
			SimY = NextLeftY;

			NextLeftFound = true;
		}
		else if (NextBounceX > RightPaddleX)
		{
			NextRightY = SimY + (RightPaddleX - SimX) * SimDy / SimDx;
			SimDx = -SimDx;
			SimX = RightPaddleX;
			SimY = NextRightY;

			NextRightFound = true;
		}
		else
		{
			// Otherwise keep bouncing!
			SimX = NextBounceX;
			SimY = NextBounceY;
			SimDy = -SimDy;
		}

		/*debugPoints[numDebugPoints*2] = NextBounceX;
		debugPoints[numDebugPoints*2+1] = NextBounceY;
		numDebugPoints++;*/
	}

	PredictedLeftY = NextLeftY;
	PredictedRightY = NextRightY;
}

Player::Side Ball::IsGoingTo()
{
	if(dx > 0)	return Player::Side_Right;

	return Player::Side_Left;
}


bool Ball::IsOutOfBounds()
{
	return OutOfBoundsSide() != Player::Side_Undefined; // Fixed
}

Player::Side Ball::OutOfBoundsSide()
{
	if(posX <= 0)	return Player::Side_Left;
	if(posX >= 800)	return Player::Side_Right;

	return Player::Side_Undefined;
}

void Ball::ServeTo(Player::Side serveTo)
{
	if(serveTo == Player::Side_Left)
	{
		dx = -5;
		dy = -5;
	}

	if(serveTo == Player::Side_Right)
	{
		dx =  5;
		dy = -5;
	}	

	dx /= 1; // ??
	dy /= 1;

	UpdateDestiny();
}

int Ball::GetPredictedLeftY() const
{
	return PredictedLeftY;
}

int Ball::GetPredictedRightY() const
{
	return PredictedRightY;
}
