CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
LDFLAGS = -lncurses


all:
	$(CC) $(CFLAGS) *.c $< -o tetris $(LDFLAGS)