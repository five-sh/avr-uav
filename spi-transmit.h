#include <inttypes.h>

/**
 * Initiailize the SPI bus
 *
 * lsbfirst - 0 - most significant bit is transmitted first
 *
 * master   - 0 - use master mode
 *	    - 1 - use slave mode
 * mode - set the transfer mode
 *		   mode   leading clock edge   trailing clock edge
 *                 -----------------------------------------------
 *                 0      sample (rising)      setup  (falling)
 *                 1      setup  (rising)      sample (falling)
 *                 2      sample (falling)     setup  (rising)
 *                 3      setup  (falling)     sample (rising)
 *
 * clkrate - bus clock rate
 *		      rate   speed
 *                    ------------
 *                    0      CPUCLK/4
 *                    1      CPUCLK/16
 *                    2      CPUCLK/64
 *                    3      CPUCLK/128
 *
 * dbl_clkrate - 1 - double the clock rate in master
 */
void spi_init(uint8_t lsbfirst, uint8_t master, uint8_t mode,
	      uint8_t clkrate, uint8_t dbl_clkrate);

uint8_t spi_send(uint8_t data);
