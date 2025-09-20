
CC=gcc
CFLAGS=-O2 -Wall -Wextra -Iinclude
LDFLAGS=
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
BIN=bin/pm_daemon

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f src/*.o $(BIN)
