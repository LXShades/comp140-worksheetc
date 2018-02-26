#include "ArduinoController.h"

#include "serial/serial.h"
#include <vector>
#include <sstream>

#include <SDL.h>

void ArduinoController::Init()
{
	LeftKnob = 0.0f;
	RightKnob = 0.0f;
	NumPollsReceived = 0;

	// initialise serial
	std::vector<serial::PortInfo> DevicesFound = serial::list_ports();

	IsDeviceConnected = false;
	AreYouCerealBro = nullptr;

	for (serial::PortInfo& Device : DevicesFound)
	{
		const char* Port = Device.port.c_str();

		try 
		{
			AreYouCerealBro = new serial::Serial(Port, 115200, serial::Timeout::simpleTimeout(300));

			if (AreYouCerealBro->isOpen()) 
			{
				IsDeviceConnected = true;
				break;
			}
			else
			{
				delete AreYouCerealBro;
				AreYouCerealBro = nullptr; // copypasta?
			}
		}
		catch(std::exception &e)
		{
			delete AreYouCerealBro;
			AreYouCerealBro = nullptr;
		}
	}

	if (AreYouCerealBro)
	{
		// Set the timeout so if we freeze or are too slow, the controls don't get infinitely delayed
		/*serial::Timeout WaitTime = AreYouCerealBro->getTimeout();

		WaitTime.read_timeout_constant = 1;
		WaitTime.read_timeout_multiplier = 2;
		AreYouCerealBro->setTimeout(WaitTime);*/
	}
}

void ArduinoController::Shutdown()
{
	if (AreYouCerealBro)
	{
		AreYouCerealBro->flush();
		AreYouCerealBro->close();
	}

	delete AreYouCerealBro;
}

void ArduinoController::UpdateIn()
{
	if (!AreYouCerealBro)
	{
		// It isn't working? You can't be cereal!
		return;
	}

	// Receive input from the arduino
	std::string Line = "";

	while (AreYouCerealBro->available() >= 10)
	{
		Line = AreYouCerealBro->readline();

		size_t MiddlePoint = Line.find("-", 0);
		printf(Line.c_str());

		if (!MiddlePoint)
		{
			continue;
		}

		// Convert the string to a pair of numbers
		std::stringstream FirstNumber(Line.substr(0, MiddlePoint));
		std::stringstream SecondNumber(Line.substr(MiddlePoint + 1));
		int FirstInt = 0, SecondInt = 0;

		FirstNumber >> FirstInt;
		SecondNumber >> SecondInt;

		// Set the knob values
		LeftKnob = (float)(POTENTIOMETER_MAX - FirstInt) / (float)POTENTIOMETER_MAX;
		RightKnob = (float)SecondInt / (float)POTENTIOMETER_MAX;

		// Clamp them to 1.0f
		if (LeftKnob > 1.0f)
		{
			LeftKnob = 1.0f;
		}
		if (LeftKnob < 0.0f)
		{
			LeftKnob = 0.0f;
		}

		if (RightKnob > 1.0f)
		{
			RightKnob = 1.0f;
		}
		if (RightKnob < 0.0f)
		{
			RightKnob = 0.0f;
		}
	};
}

void ArduinoController::UpdateOut()
{
	if (AreYouCerealBro)
	{
		// Prepare LED status
		// Show scores if desired
		if (NextScoreDisplayTime)
		{
			// Show scores on each side while being extremely frustrated with copypasta significantly enough to write unreadable code
			int Score[] = {NextLeftScore, NextRightScore};
			int UpperLight[] = {LED_TL, LED_TR};
			int LowerLight[] = {LED_BL, LED_BR};
			for (int Side = 0; Side < 2; ++Side)
			{
				// Light values for each score <= 2
				int UpperValues[] = {0, 255, 255};
				int LowerValues[] = {0,   0, 255};

				if (Score[Side] <= 2)
				{
					OutputData.LedBrightnesses[UpperLight[Side]] = UpperValues[Score[Side]];
					OutputData.LedBrightnesses[LowerLight[Side]] = LowerValues[Score[Side]];
				}
				else
				{
					OutputData.NumFlashes[Side] = 5;
				}
			}
		}

		// Send data to the arduino
		// Replace any bytes that'll interfere with the terminator
		for (int i = 0; i < sizeof (OutputData) - 1; ++i)
		{
			if (((uint8_t*)&OutputData)[i] == 127)
			{
				((uint8_t*)&OutputData)[i] = 128;
			}
		}

		AreYouCerealBro->write((uint8_t*)&OutputData, sizeof (OutputData));

		// If scores are being displayed, wait
		if (NextScoreDisplayTime)
		{
			SDL_Delay(NextScoreDisplayTime);
		}

		// Cleanup any events
		OutputData.NumFlashes[0] = OutputData.NumFlashes[1] = 0;

		NextScoreDisplayTime = 0;
	}
}

bool ArduinoController::IsControllerWorking()
{
	return IsDeviceConnected && NumPollsReceived > 0;
}
