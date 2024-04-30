# Makefile for OpenCV C++ program

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# OpenCV libraries
OPENCV_LIBS = `pkg-config --cflags --libs opencv4`

# Source files
SRCS = main.cpp

# Executable name
TARGET = addTexture

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(OPENCV_LIBS)

.PHONY: clean

clean:
	rm -f $(TARGET)
