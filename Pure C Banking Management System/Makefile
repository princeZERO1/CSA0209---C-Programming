# Makefile for the Banking Management System

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = bank_system

# List of object files
OBJS = main.o menu.o login.o account.o transaction.o file.o utility.o

# Default rule to build the executable
all: $(TARGET)

# Linking the object files to create the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compiling source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove binaries and temporary files
clean:
	rm -f $(OBJS) $(TARGET) temp.dat

# 'clean_data' rule to reset the application state (USE WITH CAUTION)
clean_data:
	rm -f $(OBJS) $(TARGET) temp.dat accounts.dat transactions.dat
