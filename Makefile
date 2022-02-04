main:
	gcc -std=gnu99 -Wall -g -o smallsh smallsh.c command_prompt.c command_handlers.c commands_built_in.c commands_external.c memory.c

clean:
	rm -f smallsh