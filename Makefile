CC = gcc
CFLAGS = -Wall -Wextra -Wno-unreachable-code-generic-assoc -Wno-unused-value -Wno-unused-parameter -std=c11 -g
INCLUDES = -Isource -Isource/core

# Find all source `*.c` files (excluding `_test.c`)
SRC_FILES = $(filter-out %_test.c, $(shell find source -name '*.c'))

# Targets
ion: $(SRC_FILES)
	$(CC) $(CFLAGS) $(INCLUDES) -o ion.exe $(SRC_FILES)

headless_ts_test: source/headless_ts_test.c source/frontend/ts/type_system.c
	$(CC) $(CFLAGS) $(INCLUDES) -o headless_ts_test.exe $^

clean:
	rm -rf ion.* headless_ts_test.* *.exe

.PHONY: ion headless_ts_test clean
