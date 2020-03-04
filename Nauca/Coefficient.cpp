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

	// Создание и запуск отсоединенного потока генерации значения коэффициента
	std::thread valueThread(&Coefficient::ChangeValueThread, this);
	valueThread.detach();
}

Coefficient::~Coefficient()
{
	aborted = true;
}

/**
 * Запуск потока изменения значения коэффициента.
 */
void Coefficient::StartChanging()
{
	threadWorking = true;
}

/**
 * Остановка потока изменения значения коэффициента.
 */
void Coefficient::StopChanging()
{
	threadWorking = false;
}	

/**
 * Ручная установка значения коэффициента.
 * @param val - значение коэффициента.
 */
void Coefficient::SetValue(double val)
{
	if (threadWorking) {
		throw "Нельзя устанавливать ручное значение при запущенном потоке генерации значения!";
	}
	else {
		value = val;
	}
}

/**
  * Получение статуса потока генерации значения коэффициента.
  * @return true если поток генерации коэффициента запущен.
  */
bool Coefficient::IsWorking()
{
	return threadWorking;
}

/**
  * Получение текущего значения коэффициента.
  * @return значение коэффициента.
  */
double Coefficient::GetValue()
{
	return value;
}

/**
  * Функция потока генерации значения коэффициента.
  */
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