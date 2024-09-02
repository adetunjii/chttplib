STD=-pedantic

# Compiler flags
CFLAGS = -Wall -Wextra -Werror

# directories
SRC_DIR = src
BINARY_DIR = BINARY_DIR

SRCS = $(shell find $(SRC_DIR) -name '*.c')

test-request:
	$(CC) $(STD) -DTEST_REQUEST -o bin/request $(SRCS)

format:
	clang-format -style=file -i **/*.c **/*.h