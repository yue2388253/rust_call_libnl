CC = gcc

INC_PATH = /usr/include/libnl3

CFLAGS := -Wall -fpic -I$(INC_PATH)

MKDIR_P = mkdir -p
OUT_DIR = build

OBJS = $(OUT_DIR)/rate_control.o

SRCS = src/rate_control.c

TARGET = $(OUT_DIR)/lib_setrate.a

all: $(TARGET)

run:
	make
	cargo run

$(TARGET): $(OBJS)
	rm -f $@
	ar rcs $@ $(OBJS)
	rm -f $(OBJS)

$(OBJS) : $(SRCS)
	$(MKDIR_P) $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OUT_DIR) $(OBJS)
	rm -rf $(CARGO_BUILD_DIR)
	cargo clean

.PHONY: clean all run