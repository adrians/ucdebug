#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include "dbg.h"

// the buffer where the data is stored
static char uart_buffer_rx[16];

// Offset to the first uninitialized position in the uart_buffer_rx array.
static uint8_t uart_buffer_rx_pos;

void dbg_init() {
	// Configure UART 9600/8-N-1
	UCSR0A = 0;
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	UBRR0 = 103;

	sei();
}

ISR(USART_RX_vect){
	uint8_t data = UDR0;

	if ((data != '\n') && (data != '\r')){
		// happy path, buffer the data and continue
		uart_buffer_rx[uart_buffer_rx_pos] = data;
		uart_buffer_rx_pos++;

		// TODO: handle buffer overruns
	} else {
		if(uart_buffer_rx_pos > 0) {
			// Received a full command, parse the buffer and act on it:

			// Make sure the string is zero-terminated.
			uart_buffer_rx[uart_buffer_rx_pos] = 0;
			// Reset the buffer position.
			uart_buffer_rx_pos = 0;

			uint16_t data1, data2;
			char action;
	
			// TODO: check for malformed commands
			sscanf( (char*) uart_buffer_rx, "%c %x %x", &action, &data1, &data2);

			if (action == 's') {
				// set command
				*(uint8_t*)data1 = (uint8_t) data2;

			} else if (action == 'g') {
				// get command
				char tx_buffer[8];
				uint8_t buffer_len, i = 0;

				buffer_len = sprintf(tx_buffer, "%02x\n", *(uint8_t*)data1);
				while(i < buffer_len){
					while(!(UCSR0A & (1<<UDRE0)));
					UDR0 = tx_buffer[i];
					i++;
				}
			}
		}
	}
}
