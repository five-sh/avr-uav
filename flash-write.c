#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>
#include "uart-receive.h"
#include "spi-transmit.h"

#define BAUD_RATE 9600

#define MSG_BUFFER_SIZE (1 << 6)

static void init()
{
	uart_init(BAUD_RATE);
	spi_init(0, 1, 0, 3, 0);
}

static void write_byte_to_flash(uint32_t address, uint8_t data)
{
	spi_send((address >> 16) & 0xFF); /* high byte */
	spi_send((address >> 8) & 0xFF); /* middle byte */
	spi_send(address & 0xFF); /* low byte */
	spi_send(data);
}

int main()
{
	init();

	while (1) {
		uint32_t address = 0x0000;
		int8_t message[MSG_BUFFER_SIZE];
		int8_t received_char;
		uint8_t i = 0;

		while (1) {
			received_char = uart_getchar();
			if (received_char == '\n') {
				message[i] = '\0';
				break;
			}
			message[i++] = received_char;
		}

		for (i = 0; message[i]; i++)
			write_byte_to_flash(address, message[i]);
	}
	_delay_ms(1000);

	return 0;
}
