#pragma once
#include "serial/serial.h"

class ArduinoController
{
public:
	void Init();
	void Shutdown();

	void Update();

	inline float GetLeftKnob() const;
	inline float GetRightKnob() const;

	void SetLEDBrightness(int LedIndex, float Brightness);
	void SetLEDBlink(int LedIndex, float MinBrightness, float MaxBrightness, float BlinkRate);

	bool IsControllerWorking();

private:
	serial::Serial* AreYouCerealBro;

	float LeftKnob, RightKnob;
	bool IsDeviceConnected;

	int NumPollsReceived;
};

inline float ArduinoController::GetLeftKnob() const
{
	return LeftKnob;
}

inline float ArduinoController::GetRightKnob() const
{
	return RightKnob;
}

inline void ArduinoController::SetLEDBrightness(int LedIndex, float Brightness)
{
}
