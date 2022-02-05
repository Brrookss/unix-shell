#ifndef MEMORY_H_
#define MEMORY_H_

#include "command_prompt.h"
#include "shell_process.h"

void deallocateInput(char *);
void deallocateCommandStruct(struct Command *);
void deallocateShellProcessStruct(struct ShellProcess *);

#endif