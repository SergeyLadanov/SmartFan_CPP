#include "pid.hpp"

// Инициализация объекта ПИД регулятора
PID::PID(float Kp, float Ki, float Kd)
{
    Kp = Kp;
    Ki = Ki;
    Kd = Kd;
    Dt = PID_SAMPLE_TIME;
    MaxControlValue = PID_MAX_CONTROL_VALUE;
    MinControlValue = -PID_MIN_CONTROL_VALUE;
    MaxIntegralValue = PID_INTEGRAL_RANGE;
    MinIntegralValue = -PID_INTEGRAL_RANGE;
    P = 0;
    I = 0;
    D = 0;
    Xn = 0;
    Xn_1 = 0;
    TargetVal = 0;
    Reverse = false;
}

// Установка ограничений интегрирующей части регулятора
void PID::SetIntegralRange(float value)
{
    MaxIntegralValue = value;
    MinIntegralValue = -value;
}

// Установка ограничений выходного сигнала регулятора
void PID::SetOutputRange(float minValue, float maxValue)
{
    MaxControlValue = maxValue;
    MinControlValue = minValue;
}

// Установка времени дискретизации
void PID::SetSampleTime(float sampleTimeVal)
{
    Dt = sampleTimeVal;
}

// Установка инверсии выходного сигнала
void PID::SetReverse(bool state)
{
    Reverse = state;
}

// Установка задания
void PID::SetTarget(float value)
{
    TargetVal = value;
}

// Обработчик ПИД регулятора
float PID::Handle(float inputVal)
{
    float out;

    Xn = TargetVal - inputVal;
    P = Xn;
    I = I + Xn * Dt;

    // Ограничение сигнала интегрирующей части
	if (I > MaxIntegralValue)
    {
        I = MaxIntegralValue;
    }
    if (I < MinIntegralValue)
    {
        I = MinIntegralValue;
    }

    D = (Xn - Xn_1) / Dt;
    Xn_1 = Xn;

    out = P * Kp + I * Ki + D * Kd;

    // Управление инверсией выходного сигнала
    if (Reverse)
    {
        out = -out;
    }
     // Ограничение выходного сигнала
    if (out > MaxControlValue)
    {
        out = MaxControlValue;
    }
    if (out < MinControlValue)
    {
        out = MinControlValue;
    }

    return out;
}



