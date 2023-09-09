#include <inttypes.h>

#define UART_BAUD_SELECT(baud_rate, xtal_cpu) \
	(((xtal_cpu) + 8UL * (baud_rate)) / (16UL * (baud_rate)) - 1UL)

#define UART_BAUD_SELECT_DOUBLE_SPEED(baud_rate, xtal_cpu) \
	(((((xtal_cpu) + 4UL * (baud_rate)) / (8UL * (baud_rate)) - 1UL)) | \
	  0x8000)

#ifndef UART_RX_BUFFER_SIZE
#define UART_RX_BUFFER_SIZE (1 << 5)
#endif

#define UART_FRAME_ERROR	0x1000
#define UART_OVERRUN_ERROR	0x0800
#define UART_PARITY_ERROR	0x0400
#define UART_BUFFER_OVERFLOW	0x0200
#define UART_NO_DATA		0x0100

/* Receiving */
void uart_init(uint16_t baud_rate);
uint16_t uart_getchar(void);
