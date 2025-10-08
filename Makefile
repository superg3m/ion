CC = gcc
CFLAGS = -Wall -Wextra -Wno-unreachable-code-generic-assoc -Wno-unused-value -Wno-unused-parameter -std=c11 -g
INCLUDES = -Isource -Isource/core

# Find all source `*.c` files (excluding `_test.c`)
SRC_FILES = $(filter-out %_test.c, $(shell find source -name '*.c'))

# Targets
ion: $(SRC_FILES)
	$(CC) $(CFLAGS) $(INCLUDES) -o ion.exe $(SRC_FILES)

clean:
	rm -rf ion.* *.exe

.PHONY: ion clean
