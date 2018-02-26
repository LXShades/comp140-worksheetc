#pragma once
#include "Object.h"

class Player : public Object
{
public:
	void virtual init();
	void virtual Move();
	
	enum Side{Side_Undefined,Side_Left,Side_Right};
	void SetSide(Side eSide);

	void SetController(const class ArduinoController* controller);
	
	inline float YToKnob(float Y) const;
	inline float KnobToY(float Knob) const;
	
private:
	Side mySide;
	int speed;

	const class ArduinoController* controller;
};

inline float Player::KnobToY(float Knob) const
{
	return (100 + (600 - 100 - h) * Knob);
}

inline float Player::YToKnob(float Y) const
{
	return (Y - 100) / (600 - 100 - h);
}