OS := $(shell uname)
ifeq ($(OS),Darwin)
CC = clang
else
CC = gcc
endif

CFLAGS = -g
LIBS = -lfl
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
