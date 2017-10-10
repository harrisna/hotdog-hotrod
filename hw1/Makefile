OS := $(shell uname)
ifeq ($(OS),Darwin)
CC = clang
CFLAGS = -g -Wall -Werror
else
CC = gcc
CFLAGS = -g -Wall -Werror
endif

LIBS =
INCLUDE =
SRC = src/lex.yy.c src/myshell.c
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
TARGET = mysh



all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(INCLUDE)

obj/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

clean:
	rm -f obj/*.o $(TARGET)
