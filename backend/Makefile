all: demo.hex

demo.hex: demo.elf
	avr-objcopy  -j .text -j .data -O ihex $^ $@
	avr-size $@

demo.o: demo.c
	avr-gcc -mmcu=atmega328p -Os -Wall -Wextra -Wpedantic -DF_CPU=16000000 -c -o $@ $^

dbg.o: dbg.c
	avr-gcc -mmcu=atmega328p -Os -Wall -Wextra -Wpedantic -DF_CPU=16000000 -c -o $@ $^

demo.elf: demo.o dbg.o
	avr-gcc -mmcu=atmega328p -o $@ $^

clean:
	rm -rf *.o *.elf *.hex

flash:
	sudo avrdude -c arduino -P /dev/ttyUSB0 -b 115200 -p atmega328p -U flash:w:lab1.hex:a
