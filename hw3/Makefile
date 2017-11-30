LIBS =
CFLAGS = -g -O3 -std=c++11
INCLUDE =

OS := $(shell uname)
ifeq ($(OS),Darwin)
CC = clang++
LIBS += 
else
CC = g++
LIBS += 
endif

SRC = src/main.cpp src/scheduler.cpp src/scheduler_rts.cpp src/scheduler_mfqs.cpp src/scheduler_whs.cpp
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
TARGET = scheduler



all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(INCLUDE)

obj/%.o: src/%.cpp src/%.h
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)

clean:
	rm -f obj/*.o $(TARGET)
