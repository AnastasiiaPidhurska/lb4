PROJECT   := lab4-task1
CC        := gcc
CFLAGS    := -std=c11 -Wall -Wextra -Werror -pedantic -O2
BUILD_DIR := build
INCLUDE   := include
SRC_DIR   := src
BIN       := $(BUILD_DIR)/task1

SRC := $(SRC_DIR)/timer.c \
       $(SRC_DIR)/main.c

OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

.PHONY: all run clean distclean dirs

all: dirs $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

run: all
	@echo "--- Запуск ---"
	$(BIN)

clean:
	rm -rf $(BUILD_DIR)

distclean: clean
	rm -f *~ $(SRC_DIR)/*~ $(INCLUDE)/*~

dirs:
	@mkdir -p $(BUILD_DIR)
