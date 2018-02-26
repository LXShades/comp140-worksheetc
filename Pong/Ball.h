#pragma once
#include "Object.h"
#include "Player.h"

class Ball : public Object
{
public:
	void virtual init();
	void virtual Move();
	
	void SetController(class ArduinoController* Controller);

	void PlayerReturns();
	bool IsOutOfBounds();
	Player::Side OutOfBoundsSide();
	Player::Side IsGoingTo();

	void ServeTo(Player::Side serveTo);

	void UpdateDestiny();
	int GetPredictedLeftY() const;
	int GetPredictedRightY() const;

	int debugPoints[50];
	int numDebugPoints;

private:
	float dx,dy;
	int PredictedLeftY, PredictedRightY;

	class ArduinoController* Controller;
};