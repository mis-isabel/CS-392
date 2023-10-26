#Isabel Sutedjo
#I pledge my honor that I have abided by the Stevens Honor System.
CC = gcc
CFLAGS = -Wall

minishell: minishell.o
	$(CC) $(CFLAGS) -o $@ $^

minishell.o: minishell.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f minishell.o minishell