CC = gcc
FLAGS = -std=c11 -pedantic-errors
SRC = src/

MINISHELL = $(wildcard $(SRC)*.c)
OBJS = $(patsubst %.c, %.o, $(MINISHELL))

.PHONY: all debug clean

all: minishell

debug: FLAGS += -DDEBUG
debug: all

minishell: $(OBJS)
	$(CC) $(FLAGS) $^ -o $@

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f minishell $(OBJS)
