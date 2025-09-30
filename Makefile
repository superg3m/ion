CC := gcc

SRC_DIR := source
CORE_DIR := $(SRC_DIR)/core
FRONTEND_DIR := $(SRC_DIR)/frontend

CFLAGS := -Wall -Wextra -std=c11 -g \
          -I$(SRC_DIR) \
          -I$(CORE_DIR) \

OBJ_DIR := build

# Find all .c files
SRCS := $(wildcard $(SRC_DIR)/*.c) \
        $(wildcard $(CORE_DIR)/*.c) \
        $(wildcard $(FRONTEND_DIR)/*/*.c)

# Convert .c -> .o under build/
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
OBJS := $(patsubst $(CORE_DIR)/%.c,$(OBJ_DIR)/core/%.o,$(OBJS))
OBJS := $(patsubst $(FRONTEND_DIR)/%/%.c,$(OBJ_DIR)/frontend/%/%.o,$(OBJS))

TARGET := ion.exe

all: $(TARGET)

# Link final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Pattern rules for each source tree
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/core/%.o: $(CORE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/frontend/%/%.o: $(FRONTEND_DIR)/%/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
