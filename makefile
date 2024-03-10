# Compiler
CC = g++

# Compiler flags
CFLAGS = -w -std=c++11	

# Source files
SRCS = $(wildcard *.cpp)

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = myprogram

# Default target
all: $(TARGET)

# Compile source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lm


# Clean up object files and the target executable
clean:
	del	/F	/Q $(OBJS) $(TARGET)
