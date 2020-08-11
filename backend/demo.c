#include <avr/io.h>
#include "dbg.h"

int main() {
	dbg_init();

	DDRB = (1<<PB5);
	PORTB = 0;

	while(1) {;}
		
	return 0; // don't let the compiler complain about this
}

