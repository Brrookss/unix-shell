#ifndef MEMORY_H_
#define MEMORY_H_

#include "command_prompt.h"
#include "shell_process.h"

void deallocateCommand(struct Command*);
void deallocateInput(char*);
void deallocateShellProcess(struct ShellProcess*);

#endif