#include "motor.h"

#include "tim.h"

void Motor_Start(void)
{
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}


void Motor_Stop(void)
{
    HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
}


void Motor_SetFrequency(uint16_t freq)
{
    if (freq > 16000) return;
    TIM2->ARR = 32000 / freq;
    TIM2->CCR1 = 16000 / freq;
}
