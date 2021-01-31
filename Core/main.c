#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "pid.h"


#define PIN        15U
 
#define RANGE_MAX           900
#define RANGE_MIN           600
 
#define TEMPERATURE_MAX      80.0f
#define TEMPERATURE_MIN      60.0f

#define SAMPLE_TIME 1.0f
#define PID_KI 3.0f
#define PID_KD 1.0f
#define PID_KP ((RANGE_MAX - RANGE_MIN) / (TEMPERATURE_MAX - TEMPERATURE_MIN))

// Функция получения температуры процессора
static float getTemperature() {
  FILE *fp;
  char filename[] = "/sys/devices/virtual/thermal/thermal_zone0/temp";
  char filestring[100];
  float temperature = 0;

  if ((fp = fopen(filename, "r")) == NULL)
  {
    printf("Не удалось открыть файл\r\n");
    return 0;
  }

  if (fgets(filestring, 100, fp) == NULL ) // считывание строки из файла
  {
    printf("Не удалось считать данные из файла\r\n");
    fclose(fp);
    return 0;
  }

  fclose(fp);
  temperature = atof(filestring);

  return temperature/1000;
}

// Основная программа
int main(void)
{
    float temperature;
    int pwmValue;
    uint8_t pwmStopped = PID_TRUE;
    PID_Obj hpid;

    PID_Init(&hpid, PID_KP, PID_KI, PID_KD);
    PID_SetIntegralRange(&hpid, RANGE_MAX);
    PID_SetOutputRange(&hpid, RANGE_MIN, RANGE_MAX);
    PID_SetSampleTime(&hpid, PID_SAMPLE_TIME);
    PID_SetReverse(&hpid, PID_TRUE);
    PID_SetTarget(&hpid, TEMPERATURE_MIN);


    if (wiringPiSetup() == 0) 
    {
        while (1) 
        {
            temperature = getTemperature();
            printf("Температура %2.1f\r\n", temperature);
            pwmValue = (int) PID_Handle(&hpid, temperature);
            printf("Управляющее значение: %d\r\n", pwmValue); 

            if (temperature >= TEMPERATURE_MIN && pwmStopped) 
            {
                printf("Включение ШИМ\r\n");
                softPwmCreate(PIN, pwmValue, RANGE_MAX);
                pwmStopped = PID_FALSE;
            } 
            else if (temperature >= (TEMPERATURE_MIN - 5) && !pwmStopped) 
            {
                printf("ШИМ активен\r\n");
                softPwmWrite(PIN, pwmValue);
            } 
            else 
            {
                if (!pwmStopped) 
                {
                    printf("Остановка ШИМ\r\n");
                    softPwmStop(PIN);
                }
                pwmStopped = PID_TRUE;
            }

            usleep(SAMPLE_TIME * 1000 * 1000);
        }
    }

    return 0;  
}