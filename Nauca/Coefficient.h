#pragma once
#include <atomic>
#include <thread>
#include <chrono>


class Coefficient
{
public:
	Coefficient(double min, double max, double freq = 1);
	~Coefficient();

	void StartChanging();
	void StopChanging();
	bool IsWorking();
	void SetValue(double val);
	double GetValue();

private:
	std::atomic<double> minValue;
	std::atomic<double> maxValue;
	std::atomic<double> frequency;
	std::atomic<double> value;

	std::atomic<bool> threadWorking;
	std::atomic<bool> aborted;
	
	int secondsPass;

	void ChangeValueThread();
};