#include "ArduinoController.h"

#include "serial/serial.h"
#include <vector>

void ArduinoController::Init()
{
	LeftKnob = 0.0f;
	RightKnob = 0.0f;
	NumPollsReceived = 0;

	// initialise serial
	std::vector<serial::PortInfo> devicesFound = serial::list_ports();

	IsDeviceConnected = false;
	AreYouCerealBro = nullptr;

	for (serial::PortInfo& device : devicesFound)
	{
		const char* port = device.port.c_str();

		try 
		{
			AreYouCerealBro = new serial::Serial(port, 115200, serial::Timeout::simpleTimeout(300));

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
}

void ArduinoController::Shutdown()
{
	delete AreYouCerealBro;
}

void ArduinoController::Update()
{
	// Receive input from the arduino

	// Send LED status to the arduino
}

bool ArduinoController::IsControllerWorking()
{
	return IsDeviceConnected && NumPollsReceived > 0;
}
