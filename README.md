The code to write to an external SPI flash memory, connected to an ATmega32.
The write buffer has NMEA 0183 message strings which are received from a
NavIC module - Skytarq Venus634FLPx GPS Receiver. <br />

The message reading is done thorugh UART and the message writing is done
through SPI. <br />

The header files and their respective implementations are taken from various
resources, but mostly are partial implementations of the avr-gcc-libraries.
