#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "command_prompt.h"
#include "shell_process.h"

void deallocateCommand(struct Command*);
void deallocateInput(char*);
void deallocateShellProcess(struct ShellProcess*);

#endif /* __MEMORY_H__ */