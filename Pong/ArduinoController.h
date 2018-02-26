#pragma once
#include "serial/serial.h"

const int LED_TL = 0, LED_TR = 1, LED_BL = 2, LED_BR = 3;

const int POTENTIOMETER_MAX = 300;

class ArduinoController
{
public:
	void Init();
	void Shutdown();

	void UpdateIn();
	void UpdateOut();

	inline float GetLeftKnob() const;
	inline float GetRightKnob() const;
	inline void SetKnobs(float Left, float Right); // test

	void SetBallPrediction(float KnobFactor, int Direction);
	
	void SetLEDBrightness(int LedIndex, float Brightness);
	void FlashLEDs(int NumFlashes = 1);

	void FlashLeftLEDs(int NumFlashes);
	void FlashRightLEDs(int NumFlashes);

	void ShowScores(int WaitTime, int LeftScore, int RightScore);

	bool IsControllerWorking();


private:
	serial::Serial* AreYouCerealBro;

	float LeftKnob, RightKnob;
	bool IsDeviceConnected;

	int NumPollsReceived;

#pragma pack(push, 1)
struct ArduinoOutputData {
	uint8_t NumFlashes[2]; // Number of flashes for each side in a flash sequence
	uint8_t LedBrightnesses[4]; // Use LED_TL etc
	uint8_t NullTerminator;
}	OutputData;
#pragma pack(pop)

	int NextScoreDisplayTime;
	int NextLeftScore;
	int NextRightScore;
};

inline float ArduinoController::GetLeftKnob() const
{
	return LeftKnob;
}

inline float ArduinoController::GetRightKnob() const
{
	return RightKnob;
}

inline void ArduinoController::SetBallPrediction(float KnobFactor, int Direction)
{
	if (Direction < 0)
	{
		SetLEDBrightness(LED_TL, KnobFactor);
	}
}

inline void ArduinoController::SetLEDBrightness(int LedIndex, float Brightness)
{
	const int MaxBrightness = 25; // Don't use 255 as that is the null terminator for the serial data

	if (Brightness > 1.0f)
	{
		OutputData.LedBrightnesses[LedIndex] = MaxBrightness;
	}
	if (Brightness > 0.0f)
	{
		OutputData.LedBrightnesses[LedIndex] = (unsigned char)(Brightness * MaxBrightness);
	}
	else
	{
		OutputData.LedBrightnesses[LedIndex] = 0;
	}
}

inline void ArduinoController::FlashLEDs(int NumFlashes)
{
	// LEDs will flash during next UpdateOut
	OutputData.NumFlashes[0] = NumFlashes;
	OutputData.NumFlashes[1] = NumFlashes;
}

inline void ArduinoController::FlashLeftLEDs(int NumFlashes)
{
	// LEDs will flash during next UpdateOut
	OutputData.NumFlashes[0] = NumFlashes;
}


inline void ArduinoController::FlashRightLEDs(int NumFlashes)
{
	// LEDs will flash during next UpdateOut
	OutputData.NumFlashes[1] = NumFlashes;
}

inline void ArduinoController::ShowScores(int WaitTime, int LeftScore, int RightScore)
{
	// Prepare score display for next UpdateOut
	NextScoreDisplayTime = WaitTime;
	NextLeftScore = LeftScore;
	NextRightScore = RightScore;
}


inline void ArduinoController::SetKnobs(float Left, float Right) // test
{
	LeftKnob = Left;
	RightKnob = Right;
}