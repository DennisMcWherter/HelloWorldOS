# Kernel Makefile
CC=gcc
ASM=as
LINKER=ld

LFLAGS=-melf_i386
AFLAGS=--32#-march=i386
CFLAGS=-O0 -ggdb -Wall -Wextra -Wattributes -nostdlib -nodefaultlibs \
  -nostartfiles -fno-builtin -m32
#-m32
CINC=-I.

# Binary
KERNEL = bin/kernel.bin

# Sources directories and linker script
# Kind of lazy-- just try to compile/link all src
# in these dirs
LD_SRC = etc/linker.ld
ASM_SRC=$(wildcard x86/*.s)
SRC:=$(wildcard kernel/*.c)
SRC+=$(wildcard kernel/mem/*.c)
SRC+=$(wildcard kernel/interrupt/*.c)
SRC+=$(wildcard drivers/*.c)
SRC+=$(wildcard lib/*.c)


ASM_SRC:=$(patsubst %.s, %.o, $(ASM_SRC))
SRC:=$(patsubst %.c, %.o, $(SRC))

all: $(ASM_SRC) $(SRC)
	$(LINKER) -T $(LD_SRC) $(ASM_SRC) $(SRC) $(LFLAGS)
	mkdir -p ./bin/hwos/boot/grub
	cp -rf ./$(KERNEL) ./bin/hwos/boot
	cp -rf ./etc/grub.cfg ./bin/hwos/boot/grub/grub.cfg
	grub-mkrescue -o ./bin/hwos.iso ./bin/hwos
	rm -rf ./bin/hwos

  # Create floppy image
  #sudo losetup /dev/loop1 ./bin/floppy.img
  #sudo mount -o loop /dev/loop1 /mnt
  #sudo cp -rf ./bin/kernel /mnt/bin

%.o: %.c
	$(CC) -c -o $@ $^ $(CINC) $(CFLAGS)

%.o: %.s
	$(ASM) -o $@ $< $(AFLAGS)

clean:
	rm -rf $(KERNEL) bin/*.iso $(SRC) $(ASM_SRC)

