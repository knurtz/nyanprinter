#include "blinky.h"

#define ANIM_STEPS 6

uint32_t next_frame_tick = 0;
uint8_t next_frame = 0;

uint32_t wait_times[ANIM_STEPS] = {100, 100, 500, 100, 100, 1000};

void Blinky_NextFrame(void) 
{
  if (HAL_GetTick() < next_frame_tick)
    return;

  switch (next_frame)
  {
    case 0:
      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
      break;

    case 1:
      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
      break;

    case 2:
      HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
      break;
    
    case 3:
      HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
      break;

    case 4:
      HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
      break;

    case 5:
      HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
      break;
  }

  next_frame_tick = HAL_GetTick() + wait_times[next_frame];

  next_frame++;
  if (next_frame >= ANIM_STEPS - 3) next_frame = 0;
}