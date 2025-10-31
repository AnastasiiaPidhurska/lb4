PROJECT   := lab4-task2
CC        := gcc
CFLAGS    := -std=c11 -Wall -Wextra -Werror -pedantic -O2 -D_POSIX_C_SOURCE=200809L
BUILD_DIR := build
INCLUDE   := include
SRC_DIR   := src
BIN       := $(BUILD_DIR)/task2

SRC := $(SRC_DIR)/daemon_app.c \
       $(SRC_DIR)/main.c

OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

.PHONY: all run clean distclean dirs

all: dirs $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -I$(INCLUDE) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@

run: all
	@echo "--- Запуск демона ---"
	$(BIN)

clean:
	rm -rf $(BUILD_DIR)

distclean: clean
	rm -f *~ $(SRC_DIR)/*~ $(INCLUDE)/*~ /tmp/lab4_task2.log

dirs:
	@mkdir -p $(BUILD_DIR)
