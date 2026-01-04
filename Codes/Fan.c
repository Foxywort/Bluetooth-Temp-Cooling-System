#include <REGX52.H>
#include "Fan.h"

sbit FAN_IN1 = P1^0;
sbit FAN_IN2 = P1^1;
sbit FAN_ENA = P1^2;

static volatile unsigned char PWM_Duty = 0; // 0~100
static unsigned char pwm_counter = 0;       // 0~99

void Fan_Init(void)
{
    FAN_IN1 = 0;
    FAN_IN2 = 1;
    FAN_ENA = 0;

    TMOD = (TMOD & 0xF0) | 0x02; // Timer0 方式2
    TH0 = 156;                   // ~100us tick @12MHz
    TL0 = 156;
    ET0 = 1;
    EA  = 1;
    TR0 = 1;
}

void Timer0_Routine(void) interrupt 1 // 表示定时器0的中断
{
    pwm_counter++;
    if (pwm_counter >= 100) pwm_counter = 0;

    if (pwm_counter < PWM_Duty) {
        FAN_ENA = 1;
    } else {
        FAN_ENA = 0;
    }
}

void Fan_SetLevel(unsigned char level)
{
    FAN_IN1 = 0;
    FAN_IN2 = 1;

    switch (level) {
    case 0: PWM_Duty = 0;   break;          // 0%
    case 1: PWM_Duty = 70;  break;          // ~180/255 ≈ 70%
    case 2: PWM_Duty = 83;  break;          // ~210/255 ≈ 83%
    case 3: PWM_Duty = 100; break;          // 100%
    default: PWM_Duty = 0;  break;
    }
}

unsigned char Fan_GetDutyPercent(void)
{
    return PWM_Duty;
}