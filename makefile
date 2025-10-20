# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -ansi -std=c89
LDFLAGS =

# Project name
TARGET = lisp.exe

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/env.c \
       $(SRC_DIR)/errors.c \
       $(SRC_DIR)/eval.c \
       $(SRC_DIR)/file_io.c \
       $(SRC_DIR)/parser.c \
       $(SRC_DIR)/primitives.c \
       $(SRC_DIR)/s_exp.c \
       $(SRC_DIR)/tokenizer.c \
       $(SRC_DIR)/utils.c \
       $(SRC_DIR)/value.c

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Header files
HEADERS = $(SRC_DIR)/env.h \
          $(SRC_DIR)/errors.h \
          $(SRC_DIR)/eval.h \
          $(SRC_DIR)/file_io.h \
          $(SRC_DIR)/parser.h \
          $(SRC_DIR)/primitives.h \
          $(SRC_DIR)/s_exp.h \
          $(SRC_DIR)/tokenizer.h \
          $(SRC_DIR)/utils.h \
          $(SRC_DIR)/value.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Rebuild everything
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean rebuild run
