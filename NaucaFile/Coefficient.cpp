#include "stdafx.h"
#include "Coefficient.h"


Coefficient::Coefficient(double min, double max, double freq)
{
	secondsPass = 0;
	value = 0;
	threadWorking = false;
	aborted = false;

	minValue = min;
	maxValue = max;
	frequency = freq;

	std::thread valueThread(&Coefficient::ChangeValueThread, this);
	valueThread.detach();
}

Coefficient::~Coefficient()
{
	aborted = true;
}

void Coefficient::StartChanging()
{
	threadWorking = true;
}

void Coefficient::StopChanging()
{
	threadWorking = false;
}

void Coefficient::SetValue(double val)
{
	if (threadWorking) {
		throw "";
	}
	else {
		value = val;
	}
}

bool Coefficient::IsWorking()
{
	return threadWorking;
}

double Coefficient::GetValue()
{
	return value;
}

void Coefficient::ChangeValueThread()
{
	while (true) {
		if (aborted) {
			return;
		}

		if (threadWorking) {
			double p = (maxValue - minValue) / 2;
			value = p * sin(frequency * secondsPass) + (minValue + p);
			secondsPass++;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}