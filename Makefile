main:
	gcc -std=gnu99 -Wall -g -o smallsh smallsh.c command_prompt.c command_handlers.c

clean:
	rm -f smallsh