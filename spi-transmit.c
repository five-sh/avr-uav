#include <avr/io.h>
#include "spi-transmit.h"

#define SPI_PORT                        PORTB
#define SPI_DDR                         DDRB

const uint8_t SPI_MOSI                  = 5;
const uint8_t SPI_MISO                  = 6;
const uint8_t SPI_SCK                   = 7;
const uint8_t SPI_LSBFIRST_MASK         = 0b00000001;
const uint8_t SPI_MASTER_MASK           = 0b00000001;
const uint8_t SPI_MODE_MASK             = 0b00000011;
const uint8_t SPI_SPEED_MASK            = 0b00000011;
const uint8_t SPI_DB_CLKRATE_MASK       = 0b00000001;

void spi_init(uint8_t lsbfirst, uint8_t master, uint8_t mode,
	      uint8_t clkrate, uint8_t db_clkrate)
{
	/* Input */
	SPI_DDR |= ((1 << SPI_MOSI) | (1 << SPI_SCK));

	/* Output */
	SPI_DDR &= ~(1 << SPI_MISO);
	SPI_PORT |= (1 << SPI_MISO);

	/* Control */
	SPCR = (
		(1 << SPE) |
		((lsbfirst & SPI_LSBFIRST_MASK) << DORD) |
		((master & SPI_MASTER_MASK) << MSTR) |
		((mode & SPI_MODE_MASK) << CPHA) |
		((clkrate & SPI_SPEED_MASK) << SPR0)
	);

	SPSR = ((dbl_clkrate & SPI_DB_CLKRATE_MASK) << SPI2X);
}

uint8_t spi_send(uint8_t data)
{
	uint8_t res;

	SPDR = data;
	while (!(SPSR & (1 << SPIF)))
		/* wait */;
	res = SPDR;

	return res;
}
