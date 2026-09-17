CC=./tools/bin/arm-none-eabi-gcc
OBJCOPY=./tools/bin/arm-none-eabi-objcopy

CFLAGS=-mcpu=cortex-m3 -mthumb -O0 -Wall
LDFLAGS=-T linker.ld -nostdlib

all: main.bin

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

startup.o: startup.s
	$(CC) $(CFLAGS) -c startup.s -o startup.o

main.elf: main.o startup.o
	$(CC) $(CFLAGS) $(LDFLAGS) startup.o main.o -o main.elf

main.bin: main.elf
	$(OBJCOPY) -O binary main.elf main.bin

flash: all
	./stlink/build/bin/st-flash --connect-under-reset write main.bin 0x08000000
clean:
	rm -f main.o startup.o main.elf main.bin
