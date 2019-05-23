SDCC=sdcc
SDLD=sdld
OBJECTS=main.ihx

.PHONY: all clean flash

all: $(OBJECTS)

clean:
	rm -f ./out/*

flash:
#	STM8_serial_flasher -V -w $(OBJECT).ihx
	stm8flash -c stlinkv2 -p stm8l051f3 -w ./out/main.ihx
#	stm8flash -c stlinkv2 -p stm8l051f3 -v main.ihx

%.ihx: %.c
	$(SDCC) -lstm8 -mstm8 --out-fmt-ihx $(CFLAGS) $(LDFLAGS) -o ./out/ $< 
