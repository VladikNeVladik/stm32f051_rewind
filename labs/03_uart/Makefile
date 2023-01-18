#-----------------------
# Compiler/linker flags
#-----------------------

CC = arm-none-eabi-gcc

CFLAGS = \
	-std=c18 \
	-Wall \
	-Wextra \
	-Werror \
	-ffreestanding \
	-nostdlib \
	-march=armv6-m \
	-mcpu=cortex-m0 \
	-fno-pic \
	-mthumb

LDFLAGS = \
	-Wall \
	-Wextra \
	-Werror \
	-ffreestanding \
	-Wl,--start-group -lgcc -lc -lg -Wl,--end-group -Wl,--gc-sections \
	-march=armv6-m \
	-mcpu=cortex-m0 \
	-fno-pic \
	-mthumb \
	-Wl,--warn-common \
	-Wl,--fatal-warnings \
	-Wl,-z,max-page-size=8 \
	-Wl,-T,entry.lds

OBJCOPY = arm-none-eabi-objcopy

OBJFLAGS = -O binary

ifeq ($(DEBUG),1)
	CFLAGS += -g -DDEBUG_BUILD
else
	CFLAGS += -flto -Os
	LDFLAGS += -flto
endif

#-------
# Files
#-------

SOURCES = uart.c

OBJECTS = $(SOURCES:%.c=build/%.o)

ENTRY_FLASH = build/entry.o

EXECUTABLE_FLASH = loader.elf
BINARY_FLASH     = loader.bin

#---------------
# Build scripts
#---------------

all: $(ENTRY_FLASH) $(EXECUTABLE_FLASH) $(BINARY_FLASH) $(SOURCES)

$(EXECUTABLE_FLASH): $(ENTRY_FLASH) $(OBJECTS)
	$(CC) $(LDFLAGS) $(ENTRY_FLASH) $(OBJECTS) -o $@

$(BINARY_FLASH): $(EXECUTABLE_FLASH)
	$(OBJCOPY) $(OBJFLAGS) $< $@

build/%.o: %.c
	@echo cc $<
	@mkdir -p build
	$(CC) $(CFLAGS) -MMD -MT $@ -MF build/$*.d -o $@ -c $<

build/%.o: %.S
	@echo cc $<
	@mkdir -p build
	$(CC) $(CFLAGS) -MMD -MT $@ -MF build/$*.d -o $@ -c $<

clean: FORCE
	rm -rf *.elf
	rm -rf *.bin
	rm -rf *.o
	rm -rf *.d
	rm -rf build

#----------------------
# Hardware interaction
#----------------------

GDB_FLAGS = \
	--eval-command="set architecture arm" \
	--eval-command="set gnutarget elf32-littlearm" \
	--eval-command="target remote localhost:1234" \
	--eval-command="file $(EXECUTABLE_FLASH)"

flash: FORCE $(BINARY_FLASH)
	st-flash write $(BINARY_FLASH) 0x08000000

hardware: FORCE $(EXECUTABLE_FLASH)
	st-util -p 1234

gdb: FORCE $(BINARY_FLASH)
	arm-none-eabi-gdb $(GDB_FLAGS)

uart: FORCE
	sudo cp minirc.stm32f051 /etc/minicom/
	sudo minicom -D /dev/ttyUSB0 stm32f051

FORCE:
