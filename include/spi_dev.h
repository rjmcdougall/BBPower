#include <SPI.h>

#ifndef INC_SPIDEV_H_
#define INC_SPIDEV_H_

class spi_dev
{
public:
    spi_dev();
    static void begin(SPISettings s, int cs);
    static uint32_t readRegister_24(uint8_t addr);
    static uint32_t writeRegister(uint8_t addr, uint16_t data);
    static uint32_t readRegister_16(uint8_t addr);

private:
    static SPISettings spisettings;
    static int chipselectpin;
};

#endif