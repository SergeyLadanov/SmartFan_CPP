#ifndef __PID_H_
#define __PID_H_
#ifdef __cplusplus
extern "C" {
#endif

#define PID_FALSE 0
#define PID_TRUE 0xFF

// Настройки по умолчанию
#define PID_INTEGRAL_RANGE (65536.0f)
#define PID_MAX_CONTROL_VALUE (65536.0f)
#define PID_MIN_CONTROL_VALUE (-65536.0f)
#define PID_SAMPLE_TIME (1.0f)
 
#include <stdint.h>

typedef struct __PID_Obj{
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
    uint8_t Reverse;
}PID_Obj;

void PID_Init(PID_Obj *hpid, float Kp, float Ki, float Kd);
void PID_SetIntegralRange(PID_Obj *hpid, float value);
void PID_SetSampleTime(PID_Obj *hpid, float sampleTimeVal);
void PID_SetReverse(PID_Obj *hpid, uint8_t state);
void PID_SetTarget(PID_Obj *hpid, float value);
float PID_Handle(PID_Obj *hpid, float inputVal);
void PID_SetOutputRange(PID_Obj *hpid, float minValue, float maxValue);

#ifdef __cplusplus
}
#endif
#endif
