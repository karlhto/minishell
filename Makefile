BIN = bin/
SRC = src/

CC = gcc
FLAGS = -std=c11 -pedantic-errors

MINISHELL = $(wildcard $(SRC)*.c)
OBJS = $(patsubst %.c, %.o, $(MINISHELL))

.PHONY: all debug clean

all: minishell

debug: FLAGS += -DDEBUG
debug: all

minishell: $(OBJS)
	$(CC) $(FLAGS) $^ -o $(BIN)$@

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f minishell $(OBJS)
