TARGET ?= libkinart.a
BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
INCLUDE_DIR ?= ./include

CC = clang

CFLAGS ?= -Wall -Wextra -O2
CPPFLAGS += -I$(INCLUDE_DIR) -MMD -MP

SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@$(RM) -r ./build

-include $(OBJS:.o=.d)
