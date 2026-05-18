CFLAGS = -I. -Wall -Wextra -std=c11 --coverage -MMD -MD -O0 -g3

SRC_DIR = src
TEST_DIR = tests
SNIPPETS_DIR = snippets
BIN_DIR = bin
OBJ_DIR = $(BIN_DIR)/obj
RDESC_DIR := vendor/rdesc

SRCS = $(wildcard $(SRC_DIR)/*.c)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
SNIPPET_TEST_SRCS = $(wildcard $(SNIPPETS_DIR)/*.test.c)

OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.test.o,$(TEST_SRCS))
SNIPPETS_TEST_OBJS = $(patsubst $(SNIPPETS_DIR)/%.test.c,\
		       $(OBJ_DIR)/%.snippet.test.o,\
		       $(SNIPPET_TEST_SRCS))

NONMAIN_OBJS = $(filter-out $(OBJ_DIR)/main.o,$(OBJS))


TARGET = $(BIN_DIR)/main
TEST_TARGETS = $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%.test,$(TEST_SRCS))
SNIPPET_TEST_TARGETS = $(patsubst $(SNIPPETS_DIR)/%.test.c,\
			 $(BIN_DIR)/%.snippet.test,\
			 $(SNIPPET_TEST_SRCS))


default: all


RDESC_MODE := debug
RDESC_FEATURES := full

$(RDESC_DIR)/rdesc.mk:
	git clone https://github.com/metwse/rdesc.git $(RDESC_DIR) \
		--branch=v0.3.0-preview

include $(RDESC_DIR)/rdesc.mk

rdesc: $(RDESC)
	ln -s $(RDESC_INCLUDE_DIR) $@


.SECONDARY:
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) rdesc
	$(CC) $(CFLAGS) -c $< -o $@

.SECONDARY:
$(OBJ_DIR)/%.test.o: $(TEST_DIR)/%.c | $(OBJ_DIR) rdesc
	$(CC) $(CFLAGS) -c $< -o $@

.SECONDARY:
$(OBJ_DIR)/%.snippet.test.o: $(SNIPPETS_DIR)/%.test.c | $(OBJ_DIR) rdesc
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) $(RDESC) | $(BIN_DIR) rdesc
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/%.test: $(OBJ_DIR)/%.test.o $(NONMAIN_OBJS) $(RDESC) | $(BIN_DIR) rdesc
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/%.snippet.test: $(OBJ_DIR)/%.snippet.test.o $(RDESC) | $(BIN_DIR) rdesc
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@


all: $(TARGET) $(TEST_TARGETS) $(SNIPPET_TEST_TARGETS)

clean:
	$(RM) -r $(BIN_DIR) $(OBJ_DIR)


.PHONY: default all clean

-include $(OBJS:.o=.d)
-include $(TEST_OBJS:.o=.d)
-include $(SNIPPETS_TEST_OBJS:.o=.d)
