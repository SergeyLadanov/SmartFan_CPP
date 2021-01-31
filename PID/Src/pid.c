#include "pid.h"

// Инициализация объекта ПИД регулятора
void PID_Init(PID_Obj *hpid, float Kp, float Ki, float Kd)
{
    hpid->Kp = Kp;
    hpid->Ki = Ki;
    hpid->Kd = Kd;
    hpid->Dt = PID_SAMPLE_TIME;
    hpid->MaxControlValue = PID_MAX_CONTROL_VALUE;
    hpid->MinControlValue = -PID_MIN_CONTROL_VALUE;
    hpid->MaxIntegralValue = PID_INTEGRAL_RANGE;
    hpid->MinIntegralValue = -PID_INTEGRAL_RANGE;
    hpid->P = 0;
    hpid->I = 0;
    hpid->D = 0;
    hpid->Xn = 0;
    hpid->Xn_1 = 0;
    hpid->TargetVal = 0;
    hpid->Reverse = false;
}

// Установка ограничений интегрирующей части регулятора
void PID_SetIntegralRange(PID_Obj *hpid, float value)
{
    hpid->MaxIntegralValue = value;
    hpid->MinIntegralValue = -value;
}

// Установка ограничений выходного сигнала регулятора
void PID_SetOutputRange(PID_Obj *hpid, float minValue, float maxValue)
{
    hpid->MaxControlValue = maxValue;
    hpid->MinControlValue = minValue;
}

// Установка времени дискретизации
void PID_SetSampleTime(PID_Obj *hpid, float sampleTimeVal)
{
    hpid->Dt = sampleTimeVal;
}

// Установка инверсии выходного сигнала
void PID_SetReverse(PID_Obj *hpid, uint8_t state)
{
    hpid->Reverse = state;
}

// Установка задания
void PID_SetTarget(PID_Obj *hpid, float value)
{
    hpid->TargetVal = value;
}

// Обработчик ПИД регулятора
float PID_Handle(PID_Obj *hpid, float inputVal)
{
    float out;

    hpid->Xn = hpid->TargetVal - inputVal;
    hpid->P = hpid->Xn;
    hpid->I = hpid->I + hpid->Xn * hpid->Dt;

    // Ограничение сигнала интегрирующей части
	if (hpid->I > hpid->MaxIntegralValue)
    {
        hpid->I = hpid->MaxIntegralValue;
    }
    if (hpid->I < hpid->MinIntegralValue)
    {
        hpid->I = hpid->MinIntegralValue;
    }

    hpid->D = (hpid->Xn - hpid->Xn_1) / hpid->Dt;
    hpid->Xn_1 = hpid->Xn;

    out = hpid->P * hpid->Kp + hpid->I * hpid->Ki + hpid->D * hpid->Kd;

    // Управление инверсией выходного сигнала
    if (hpid->Reverse)
    {
        out = -out;
    }
     // Ограничение выходного сигнала
    if (out > hpid->MaxControlValue)
    {
        out = hpid->MaxControlValue;
    }
    if (out < hpid->MinControlValue)
    {
        out = hpid->MinControlValue;
    }

    return out;
}



