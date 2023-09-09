#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "uart-receive.h"

#define UART_RX_BUFFER_MASK (UART_RX_BUFFER_SIZE - 1)

#if (UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK)
#error RX buffer size is not a power of 2
#endif

#define UART_RECEIVE_INTERRUPT	SIG_UART_RECV
#define UART_STATUS		UCSRA
#define UART_CONTROL		UCSRB
#define UART_DATA		UDR
#define UART_UDRIE		UDRIE

/* Ring buffer */
static struct uart_rx_buffer {
	uint8_t buf[UART_RX_BUFFER_SIZE];
	uint8_t head;
	uint8_t tail;
	uint8_t last_err;
};

ISR(UART_RECEIVE_INTERRUPT)
{
	volatile struct uart_rx_buffer *uart_buf;
	uint8_t tmp_head;
	uint8_t data;
	uint8_t usr;
	uint8_t err;

	usr = UART_STATUS;
	data = UART_DATA;
	err = (usr & (_BV(FE) | _BV(DOR)));

	tmp_head = (uart_rx_buf->head + 1) & UART_RX_BUFFER_MASK;
	if (tmp_head == uart_buf->tail) {
		err = UART_BUFFER_OVERFLOW >> 8;
	} else {
		uart_buf->head = tmp_head;
		uart_buf->buf[uart_buf->head] = data;
	}
	uart_buf->last_err = err;
}

void uart_init(uint16_t baud_rate)
{
	volatile struct uart_rx_buffer *uart_buf = {
		.head = 0;
		.tail = 0;
	}

	if (baud_rate & 0x8000) {
		UART_STATUS = (1 << U2X); /* enable 2x speed */
		baud_rate &= ~0x8000;
	}
	UBRRH = (uint8_t)(baud_rate >> 8);
	UBRRL = (uint8_t)baud_rate;

	UART_CONTROL = _BV(RXCIE) | (1 << RXEN) | (1 << TXEN);

#ifdef URSEL
	UCSRC = (1 << URSEL) | (3 << UCSZ0);
#else
	UCSRC = (3 << UCSZ0);
#endif
}

uint16_t uart_getchar(void)
{
	volatile struct uart_rx_buffer *uart_buf;
	uint8_t tmp_tail;
	uint8_t data;

	if (uart_buf->head == uart_buf->tail)
		return UART_NO_DATA;
	tmp_tail = (uart_buf->tail + 1) & UART_RX_BUFFER_MASK;
	uart_buf->tail = tmp_tail;
	data = uart_buf->buf[uart_buf->tail];

	return (uart_buf->last_err << 8) + data;
}
