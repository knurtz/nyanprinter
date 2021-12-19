#include "shell.h"
#include "tim.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{ 
  if (huart->Instance == USART1)
  {
    // Return error message
    xprintf("Buffer overflow\n");

    // and start over
    Shell_Init();
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{   
    // TIM3 - count strobe interval, turn off LED3 for debugging
    if (htim->Instance == TIM3)
    {   
        HAL_TIM_Base_Stop(&htim3);
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
    }

    // TIM2 - motor steps, toggle LED4 for debugging
    if (htim->Instance == TIM2)
    {
        HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);    
    }
}