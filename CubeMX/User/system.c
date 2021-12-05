#include "system.h"
#include "shell.h"
#include "blinky.h"
#include "tim.h"

bool motor_started = false;
uint32_t next_motor_step = 0;
uint16_t freq = 100;              // Motor step frequency in Hz, max. value 1000 Hz

void System_Init(void)
{
  Shell_Init();
  HAL_GPIO_WritePin(MOT_EN_GPIO_Port, MOT_EN_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(MOT_DIR_GPIO_Port, MOT_DIR_Pin, GPIO_PIN_RESET);
}

void System_MainRoutine(void) 
{
  Shell_CheckCommand();
}
