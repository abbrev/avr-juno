ifeq ($(TARGET),avr)
F_CPU = 16000000L
CC = avr-gcc
LD = avr-gcc
CFLAGS += -mmcu=atmega328p -DF_CPU=$(F_CPU) -O2 -D_AVR_=1 #-gstabs
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
BINARY = avrsynth.elf
SRC += avrsynth.c
else
CFLAGS += -g
LDFLAGS += -lm -g
OBJCOPY = objcopy
OBJDUMP = objdump
BINARY = synth.elf
CFLAGS += -DBIG_TARGET=1
SRC += synth.c
endif

SRC += juno.c render.c wave.c bob.c voice.c audio.c #synth.c
ASRC :=

OBJ = $(SRC:.c=.o) $(ASRC:.S=.o)

CFLAGS += -Wall #-O2 #-g

all: $(BINARY) #synth.lss #bob.o voice.o
avr: all

.PHONY: obj avr
obj: $(OBJ)

%.o: %.c
	$(CC) -c $(CFLAGS) $^

%.elf: $(OBJ)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $^

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.lss: %.elf
	$(OBJDUMP) -h -S $< >$@

ifeq ($(TARGET),avr)
burn: avrsynth.hex
	@read -p 'press enter when ready to burn: '
	avrdude -P /dev/ttyUSB0 -p m328p -c arduino -v -v -U flash:w:avrsynth.hex
else
burn:
	@echo error: burn is for avr only!
	@false
endif

#.PHONY: build-wave #wave.c
build-wave: build-wave.c wave.h
	gcc -Wall -o build-wave build-wave.c -lm

wave.c: build-wave
	./build-wave >wave.c

render.o wave.o synth.o: wave.h audio.h
audio.o render.o bob.o: audio.h

clean:
	rm -f synth *.elf $(OBJ) synth.hex *.lss wave.c
