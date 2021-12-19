#include "shell.h"
#include "motor.h"

#include "usart.h"
#include "tim.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>       // for va_list var arg functions
#include <stdlib.h>


char rec_buffer[50];


extern bool motor_started;
extern uint16_t freq;

bool Shell_CheckCommand(void)
{
  // check receive buffer for newline, indicating the end of a command
  char* newline_pos = strchr(rec_buffer, '\n');

  if (newline_pos)
  {
    // end command at first newline character
    *newline_pos = '\0';
    // echo command that will be processed
    HAL_UART_Transmit(&huart1, (uint8_t*)rec_buffer, strlen(rec_buffer), -1);
    HAL_UART_Transmit(&huart1, (uint8_t*)"\n", 1, -1);
    Shell_ExecuteCommand();
    // reset everything and output prompt
    Shell_Init();
    return true;
  }

  // no command found
  return false;
}


void Shell_ExecuteCommand(void)
{
  if (strlen(rec_buffer) == 0) return;
  
  // Help command
  if (!strcmp(rec_buffer, "help"))
  {
    xprintf("List of commands:\n echo - Print message\n help - List all commands\n");
  }

  // Echo command
  else if (strstr(rec_buffer, "echo"))
  {
    HAL_UART_Transmit(&huart1, (uint8_t*)rec_buffer + 5, strlen(rec_buffer) - 5, -1);
    HAL_UART_Transmit(&huart1, (uint8_t*)"\n", 1, -1);
  }

  // Start command
  else if (!strcmp(rec_buffer, "start"))
  {
    Motor_Start();
    xprintf("Motor started\n");
  }
  
  // Stop command
  else if (!strcmp(rec_buffer, "stop"))
  {
    Motor_Stop();
    xprintf("Motor stopped\n");
  }

  // Motor freq command
  else if (strstr(rec_buffer, "freq"))
  {
    uint16_t new_freq = strtol(rec_buffer + 4, NULL, 0);
    Motor_SetFrequency(new_freq);
    xprintf("Frequency is now %i\n", new_freq);
  }

  // Strobe command
  else if (!strcmp(rec_buffer, "strobe"))
  {
    HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
    HAL_TIM_Base_Start(&htim3);
    xprintf("Strobe activated\n");
  }

  else if (strstr(rec_buffer, "cnt"))
  {
    xprintf("TIM2->CNT: %i\n", TIM2->CNT);
    xprintf("TIM3->CNT: %i\n", TIM3->CNT);
  }

  else xprintf("Unknown command.\n");
}


void Shell_Init(void)
{
  xprintf("\nNyan> ");
  Shell_ClearInputBuffer();
}

void Shell_ClearInputBuffer(void)
{
  memset(rec_buffer, 0, sizeof(rec_buffer));
  HAL_UART_AbortReceive(&huart1);
  HAL_UART_Receive_DMA(&huart1, (uint8_t*)rec_buffer, sizeof(rec_buffer));
}


// Works like normal printf, max. length 150 characters
void xprintf(const char *fmt, ...)
{
  static char buffer[151];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  int len = strlen(buffer);
  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, len, -1);
}
