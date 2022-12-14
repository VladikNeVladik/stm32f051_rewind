#-----------------------
# Compiler/linker flags
#-----------------------

CC = arm-none-eabi-gcc

CFLAGS = \
	-march=armv6-m \
	-mcpu=cortex-m0

LDFLAGS = \
	-nostdlib \
	-march=armv6-m \
	-mcpu=cortex-m0 \
	-Wl,-T,entry.lds

ifeq ($(DEBUG),1)
	CFLAGS += -g
endif

ifeq ($(QEMU),1)
	CFLAGS += -g -DINSIDE_QEMU
endif

#-------
# Files
#-------

SOURCES = blinkled.c

OBJECTS = blinkled.o

ENTRY_FLASH = build/entry.o

EXECUTABLE_FLASH = blinkled.elf
BINARY_FLASH     = blinkled.bin

#---------------
# Build scripts
#---------------

all: $(ENTRY_FLASH) $(EXECUTABLE_FLASH) $(BINARY_FLASH) $(SOURCES)

$(EXECUTABLE_FLASH): $(ENTRY_FLASH) $(OBJECTS)
	$(CC) $(LDFLAGS) $(ENTRY_FLASH) $(OBJECTS) -o $@

$(BINARY_FLASH): $(EXECUTABLE_FLASH)
	arm-none-eabi-objcopy -O binary $< $@

build/%.o: %.c
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $<

build/%.o: %.S
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.elf
	rm -rf *.bin
	rm -rf *.o
	rm -rf build

#----------------------
# Hardware interaction
#----------------------

QEMU_FLAGS = \
	-machine STM32F051-Discovery \
	-cpu cortex-m0 \
	-kernel $(EXECUTABLE_FLASH) \
	-serial stdio \
	-m 32M \
	-icount 25 \
	-gdb tcp::1234

GDB_FLAGS = \
	--eval-command="set architecture arm" \
	--eval-command="set gnutarget elf32-littlearm" \
	--eval-command="target remote localhost:1234" \
	--eval-command="file $(EXECUTABLE_FLASH)"

flash: $(BINARY_FLASH)
	st-flash write $(BINARY_FLASH) 0x08000000

qemu :  $(EXECUTABLE_FLASH)
	qemu-system-gnuarmeclipse $(QEMU_FLAGS)

hardware: $(EXECUTABLE_FLASH)
	st-util -p 1234

gdb: $(BINARY_FLASH)
	arm-none-eabi-gdb $(GDB_FLAGS)

.PHONY: all clean flash hardware gdb
