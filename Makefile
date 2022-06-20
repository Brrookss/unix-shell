main:
	gcc -std=gnu99 -Wall -g -o shell shell.c command_prompt.c command_handlers.c commands_built_in.c commands_external.c memory.c shell_process.c signal_handlers.c

clean:
	rm -f shell