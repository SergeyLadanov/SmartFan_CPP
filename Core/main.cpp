#include <wiringPi.h>
#include <softPwm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "pid.hpp"
#include <signal.h>


#define PIN        15U
 
#define RANGE_MAX           900
#define RANGE_MIN           600

// Порог включения вентилятора 
#define TEMPERATURE_MAX      80.0f
// Максимальное значение температуры (для расчета коэффициента пропорциональной части регулятора)
#define TEMPERATURE_MIN      60.0f
// Гистерезис по температуре
#define TEMPERATURE_HYSTERESIS 5.0f
// Время дискретизации температуры
#define SAMPLE_TIME 1.0f
// Коэффциент интегральной части
#define PID_KI 3.0f
// Коэффициент дифференциальной части
#define PID_KD 1.0f
// Коэффициент пропорциональной части
#define PID_KP ((RANGE_MAX - RANGE_MIN) / (TEMPERATURE_MAX - TEMPERATURE_MIN))

// Флаг запуска приложения
static bool Start = true;

// Обработчик закрытия приложения
void SigBreak_Handler(int n_signal)
{
    printf("Зыкрытие приложения...\r\n");
    Start = false;
}


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
    bool pwmStopped = true;
    PID hpid(PID_KP, PID_KI, PID_KD);

    hpid.SetIntegralRange(RANGE_MAX);
    hpid.SetOutputRange(RANGE_MIN, RANGE_MAX);
    hpid.SetSampleTime(PID_SAMPLE_TIME);
    hpid.SetReverse(true);
    hpid.SetTarget(TEMPERATURE_MIN);

    signal(SIGINT, &SigBreak_Handler);


    if (wiringPiSetup() == 0) 
    {
        while (Start) 
        {
            temperature = getTemperature();
            printf("Температура %2.1f\r\n", temperature);
            pwmValue = (int) hpid.Handle(temperature);
            printf("Управляющее значение: %d\r\n", pwmValue); 

            if (temperature >= TEMPERATURE_MIN && pwmStopped) 
            {
                printf("Включение ШИМ\r\n");
                softPwmCreate(PIN, pwmValue, RANGE_MAX);
                pwmStopped = false;
            } 
            else if (temperature >= (TEMPERATURE_MIN - TEMPERATURE_HYSTERESIS) && !pwmStopped) 
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
                pwmStopped = true;
            }

            usleep(SAMPLE_TIME * 1000 * 1000);
        }

        if (!pwmStopped)
        {
            softPwmStop(PIN);
        }

    }
    else
    {
        return 1;
    }

    return 0;  
}