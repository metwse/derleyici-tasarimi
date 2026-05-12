CFLAGS = -Wall -Wextra -std=c11 --coverage -MMD -MD -O0 -g3

SRC_DIR = src
TEST_DIR = tests
BIN_DIR = bin
OBJ_DIR = $(BIN_DIR)/obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.test.o,$(TEST_SRCS))

NONMAIN_OBJS = $(filter-out $(OBJ_DIR)/main.o,$(OBJS))


TARGET = $(BIN_DIR)/main
TEST_TARGETS = $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%.test,$(TEST_SRCS))


default: all

.SECONDARY:
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.SECONDARY:
$(OBJ_DIR)/%.test.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/%.test: $(OBJ_DIR)/%.test.o $(NONMAIN_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@


all: $(TARGET) $(TEST_TARGETS)

clean:
	$(RM) -r $(BIN_DIR) $(OBJ_DIR)


.PHONY: default all clean

-include $(OBJS:.o=.d)
-include $(TEST_OBJS:.o=.d)
