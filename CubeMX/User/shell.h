#ifndef SHELL_H
#define SHELL_H

#include "main.h"

extern bool text_mode;

// Check receive buffer for newline, returns 1 if command was found
bool Shell_CheckCommand(void);

void Shell_ExecuteCommand(void);

void Shell_Init(void);
void Shell_ClearInputBuffer(void);

// Works like normal printf, max. length 100 characters
void xprintf(const char *fmt, ...);

#endif /* SHELL_H */
