CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -Isrc
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Source files with new directory structure
SRCS = src/main.cpp \
       src/game/Game.cpp \
       src/objects/Spaceship.cpp \
       src/objects/Obstacle.cpp \
       src/managers/ResourceManager.cpp \
       src/managers/AudioManager.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f *.o

.PHONY: all clean
