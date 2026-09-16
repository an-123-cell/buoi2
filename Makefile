CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -g -c

all: firmware.bin

firmware.elf: main.o startup.o
	$(CC) -T linker.ld -nostdlib main.o startup.o -o firmware.elf

main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o

startup.o: startup.c
	$(CC) $(CFLAGS) startup.c -o startup.o

firmware.bin: firmware.elf
	$(OBJCOPY) -O binary firmware.elf firmware.bin

flash: firmware.bin
	st-flash write firmware.bin 0x8000000

clean:
	rm -f *.o *.elf *.bin
