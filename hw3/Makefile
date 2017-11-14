LIBS =
CFLAGS = -g
INCLUDE =

OS := $(shell uname)
ifeq ($(OS),Darwin)
CC = clang++
LIBS += 
else
CC = g++
LIBS += 
endif

SRC = src/main.cpp
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
TARGET = scheduler



all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(INCLUDE)

obj/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

clean:
	rm -f obj/*.o $(TARGET)
