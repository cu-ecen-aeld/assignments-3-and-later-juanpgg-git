# Makefile for the writer application

CC = cc  # Default compiler
CROSS_COMPILE ?=  # Allow for cross-compilation

# Set the target executable
TARGET = writer
# Set compilation flags
CFLAGS = -Wall -Wextra

# Default target to build the writer application
all: $(TARGET)

# Rule to compile the writer application
$(TARGET): writer.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) -o $@ $<

# Clean target to remove compiled files
clean:
	rm -f $(TARGET) *.o
