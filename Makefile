TARGET = dp32-fl

OBJS = $(TARGET).o

ifeq ($(OS),Windows_NT)
TOP := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
else
TOP := $(shell pwd)
endif

AS = arm-none-eabi-as
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

ASFLAGS = -mcpu=cortex-m0
CFLAGS = -Os -Wall -Werror -mcpu=cortex-m0 -fno-builtin -fshort-enums -fno-delete-null-pointer-checks -std=c11 -MMD -fno-toplevel-reorder
CFLAGS += -DPRINTF_INCLUDE_CONFIG_H
CFLAGS += -DGIT_HASH=\"$(GIT_HASH)\"
LDFLAGS = -mcpu=cortex-m0 -nostartfiles -Wl,-T,dp32-fl.ld -Wl,-x

INC =
LIBS =

DEPS = $(OBJS:.o=.d)

all: $(TARGET).elf
	$(SIZE) $<

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@ $(LIBS)

%.o: %.c | $(BSP_HEADERS)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

-include $(DEPS)

clean:
	rm -f $(TARGET).elf $(OBJS) $(DEPS)

