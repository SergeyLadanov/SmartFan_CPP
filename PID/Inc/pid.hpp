#ifndef __PID_H_
#define __PID_H_

// Настройки по умолчанию
#define PID_INTEGRAL_RANGE (65536.0f)
#define PID_MAX_CONTROL_VALUE (65536.0f)
#define PID_MIN_CONTROL_VALUE (-65536.0f)
#define PID_SAMPLE_TIME (1.0f)
 
#include <cstdint>

class PID{
private:
    float Kp;
    float Ki;
    float Kd;
    float Dt;
    float MaxControlValue;
    float MinControlValue;
    float MaxIntegralValue;
    float MinIntegralValue;
    float P;
    float I;
    float D;
    float Xn;
    float Xn_1;
    float TargetVal;
    bool Reverse;
public:
    PID(float kp, float ki, float kd);
    void SetIntegralRange(float value);
    void SetSampleTime(float sampleTimeVal);
    void SetReverse(bool state);
    void SetTarget(float value);
    float Handle(float inputVal);
    void SetOutputRange(float minValue, float maxValue);
};

#endif
